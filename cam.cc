#include "cam.h"

void crop_rgb(Vec *rgb)
{
	if (!rgb)
		return;

	if (rgb->x > 1)
		rgb->x = 1;
	else if (rgb->x < 0)
		rgb->x = 0;
	if (rgb->y > 1)
		rgb->y = 1;
	else if (rgb->y < 0)
		rgb->y = 0;
	if (rgb->z > 1)
		rgb->z = 1;
	else if (rgb->z < 0)
		rgb->z = 0;
}

Camera::Camera(void) {
	f_len = iw = ih = pw = ph = 0;
	eye = u = v = w = Vec();
	pixels.resizeErase(0, 0);
}

Camera::Camera(const Vec &p, const Vec &dir,
			   double d, double iwidth, double iheight,
			   int pwidth, int pheight)
{
	eye = p;
	f_len = d;
	iw = iwidth;
	ih = iheight;
	pw = pwidth;
	ph = pheight;
	set_uvw(dir);
	pixels.resizeErase(ph, pw);
}

void Camera::set_uvw(const Vec &d)
{
	(w = d*-1).normalize();
	if (1 - abs(w.y) < EPSILON)
		(u = Vec(1, 0, 0)).normalize();
	else
		(u = d.cross(Vec(0, 1, 0))).normalize();

	(v = u.cross(d)).normalize();
}

Ray Camera::gen_ray(double i, double j)
{
	Vec dir = u*iw*((i + .5)/pw - .5) + v*ih*(.5 - (j + .5)/ph) - w*f_len;

	dir.normalize();
	return Ray(eye, dir);
}

Ray Camera::jitter(double i, double j, int p, int q, int samples)
{
	double l = -iw/2.0;
	double t = ih/2.0;
	double rand_1 = (rand()%100)/100.;
	double rand_2 = (rand()%100)/100.;
	double U = l + iw*(i + ((double)(p + rand_1))/samples)/pw;
	double V = t - ih*(j + ((double)(q + rand_2))/samples)/ph;
	Vec dir = u*U + v*V - w*f_len;
	dir.normalize();
	return Ray(eye, dir);
}

void Camera::render(vector<Object *> &objects, vector<Plane *> &planes,
						const vector<Light *> &lights,
						BVH *root,
						int &p_samples,
						int &s_samples,
						int &bbox_flag)
{
	double ps2 = p_samples*p_samples;
	int stat_mod, num, count;
	stat_mod = ph*pw/100;
	num = count = 0;

	if (p_samples > 1 || s_samples > 1)
		srand(time(NULL));

	#pragma omp parallel for
	for (int y = 0; y < ph; ++y) {
		for (int x = 0; x < pw; ++x) {
			if (!(++num%stat_mod)) {
				printf("\rRendering %d%%", ++count);
				fflush(stdout);
			}

			Vec color;
			if (p_samples < 2) {
				color = ray_color(	gen_ray(x, y),
							MAX_DEPTH,
							lights,
							objects,
							planes,
							root,
							1,
							s_samples,
							bbox_flag);
			} else {
				for (int p = 0; p < p_samples; ++p) {
					for (int q = 0; q < p_samples; ++q) {
						Ray r = jitter(x, y, p, q,
								p_samples);
						color += ray_color(r,
								MAX_DEPTH,
								lights,
								objects,
								planes,
								root,
								p_samples,
								s_samples,
								bbox_flag);
					}
				}

				color /= ps2;
			}

			set_pixel(x, y, color.x, color.y, color.z);
		}
	}
}

Vec shading(const Ray &ray, const Vec &ie, const Vec &n, const Vec &il,
					Intersection &it,
					const double &max_t,
					const vector<Object *> &objects,
					const vector<Plane *> &planes,
					const Light *light,
					Material *m,
					BVH *root,
					int &flag)
{
	int shadowed = 0;
	if (root) {
		if ((root->shadow_test(ray, it,	flag)) && (it.t < max_t))
			shadowed = 1;
	} else {
		for (unsigned int i = 0; i < objects.size(); ++i) {
			if (objects[i]->intersect(ray, it, flag)) {
				if (it.t > 0.0001 && it.t < max_t) {
					shadowed = 1;
					break;
				}
			}
		}
	}

	if (!shadowed) {
		for (unsigned int i = 0; i < planes.size(); ++i) {
			if (planes[i]->intersect(ray, it, flag)) {
				if (it.t > 0.0001 && it.t < max_t) {
					shadowed = 1;
					break;
				}
			}
		}
	}

	return shadowed == 0 ? m->phong_shading(ie, n, il, light->rgb) :
							Vec(0, 0, 0);

}

Vec Camera::ray_color(const Ray &r, int depth, const vector<Light *> lights,
						const vector<Object *> objects,
						const vector<Plane *> planes,
						BVH *root,
						int p_samples,
						int s_samples,
						int &flag)
{
	if (!depth)
		return Vec(0.0, 0.0, 0.0);

	Vec retrgb(0.0, 0.0, 0.0);
	Vec rgb2(0,0,0);
	Vec ambience(0,0,0);
	Material *m = NULL;
	Intersection it, tmp;

	if (root) {
		if (root->intersect(r, it, flag))
			m = objects[it.id]->mat;
		else
			it.t = DBL_MAX;
	} else {
		for (unsigned int i = 0; i < objects.size(); ++i) {
			if (objects[i]->intersect(r, tmp, flag)) {
				if ((tmp.t > 0.0001) &&
						(tmp.t < it.t)) {
					it = tmp;
					m = objects[i]->mat;
				}
			}
		}
	}

	for (unsigned int i = 0; i < planes.size(); ++i) {
		if (planes[i]->intersect(r, tmp, flag)) {
			if ((tmp.t > .0001) && (tmp.t < it.t)) {
				it = tmp;
				m = planes[i]->mat;
			}
		}
	}

	if (!m)
		return retrgb;

	Vec p1 = it.point;
	Vec sp;			/* Will be used only for square area lights */
	Vec n = it.normal;	/* Normal at intersection */
	Vec ei = r.dir;		/* Vec from intersection to camera */
	Vec ie = ei*(-1.0);

	for (unsigned int i = 0; i < lights.size(); ++i) {
		char type = lights[i]->type;

		/* Ambient light: only calculate once */
		if (type == 'a') {
			if (depth == MAX_DEPTH)
				ambience = lights[i]->rgb*m->diff;
			continue;
		}

		/* Blinn-Phong Shading */
		Ray s_ray;
		s_ray.ori = p1;
		Vec il;
		double max_t;

		/* Square light: v, n, u are corners */
		if (type == 's') {
			for (int j = 0; j < s_samples; ++j) {
				double r1 = (rand()%100 - 50)/100.;
				double r2 = (rand()%100 - 50)/100.;
				double len2 = lights[i]->len/2;
				Vec u2 = lights[i]->n.cross(lights[i]->u);
				u2.normalize();
				sp = lights[i]->v + (u2*len2)*r1 +
						(lights[i]->u*len2)*r2;
				il = sp - p1;

				max_t = il.get_mag();
				il.normalize();
				s_ray.dir = il;
				Vec irr = shading(s_ray, ie, n,	il, it,	max_t,
						objects, planes, lights[i], m,
						root, flag);

				if (irr.get_mag() < EPSILON)
					continue;

				irr /= ((max_t+1)*(max_t+1));
				irr *= lights[i]->n.dot(il*-1);
				rgb2 += irr;
			}

			rgb2 /= s_samples;
			continue;

		/* Directional light: v is direction vector */
		} else if (type == 'd') {
			il = lights[i]->v;

		/* Point light: v corresponds to origin */
		} else {
			il = lights[i]->v - p1;
		}

		max_t = il.get_mag();
		il.normalize();

		s_ray.dir = il;

		retrgb += shading(s_ray, ie, n, il, it,	max_t, objects, planes,
						lights[i], m, root, flag);
	}

	/* Ideal specular Shading */
	Vec km = m->ideal;
	if (km.get_mag() > 0.0) {

		/* Reflection ray */
		Vec rfl = n*(2*(n.dot(ie))) - ie;
		rfl.normalize();
		Ray rray(p1, rfl);

		retrgb += km*ray_color(rray, depth-1, lights, objects, planes,
							root, 1, 1, flag);
	}

	if (depth == MAX_DEPTH)
		retrgb = retrgb + ambience;
	return retrgb + rgb2;
}

void Camera::write_exr(const char *outfile) {
	if (outfile) {
		RgbaOutputFile file(outfile, pw, ph, WRITE_RGBA);
		file.setFrameBuffer(&pixels[0][0], 1, pw);
		file.writePixels(ph);
	}
}
