#include "cam.h"

Camera::Camera(void)
{
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
	if (1 - fabs(w.y) < EPSILON)
		(u = Vec(1, 0, 0)).normalize();
	else
		(u = d.cross(Vec(0, 1, 0))).normalize();

	(v = u.cross(d)).normalize();
}

Ray Camera::gen_ray(const double i, const double j, const double offset_1 = .5,
						const double offset_2 = .5)
{
	Vec dir = u*iw*((i + offset_1)/pw - .5) + v*ih*(.5 - (j + offset_2)/ph)
								- w*f_len;
	dir.normalize();
	return Ray(eye, dir);
}

void Camera::render(vector<Object *> &objects, vector<Plane *> &planes,
						const vector<Light *> &lights,
						BVH *root,
						const int p_samples,
						const int s_samples)
{
	double ps2 = 1./(p_samples*p_samples);
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
			if (p_samples > 1) {
				for (int p = 0; p < p_samples; ++p) {
					for (int q = 0; q < p_samples; ++q) {
						double r1 = (rand()%100)/100.;
						double r2 = (rand()%100)/100.;
						Ray r = gen_ray(x, y,
							(p + r1)/p_samples,
							(q + r2)/p_samples);
						color += ray_color(r,
								MAX_DEPTH,
								lights,
								objects,
								planes,
								root,
								s_samples);
					}
				}
				color *= ps2;
			} else {
				color = ray_color(	gen_ray(x, y),
							MAX_DEPTH,
							lights,
							objects,
							planes,
							root,
							s_samples);
			}
			pixels[y][x].r = color.x;
			pixels[y][x].g = color.y;
			pixels[y][x].b = color.z;
		}
	}
}

Vec shading(const Ray &ray, const Vec &ie, const Vec &n, const Vec &il,
					Intersection &it,
					const double &max_t,
					const vector<Object *> &objects,
					const Light *light,
					Material *m,
					BVH *root)
{
	int shadowed = 0;

	if (root) {
		shadowed = root->intersect(ray.ori, ray.dir, it) && it.t < max_t;
	} else {
		for (unsigned int i = 0; i < objects.size(); ++i) {
			if (objects[i]->intersect(ray.ori, ray.dir, it)) {
				if (it.t > EPSILON && it.t < max_t) {
					shadowed = 1;
					break;
				}
			}
		}
	}

	return !shadowed ? m->phong_shading(ie, n, il, light->rgb) : Vec();
}

Vec Camera::ray_color(const Ray &r, int depth, const vector<Light *> lights,
						const vector<Object *> objects,
						const vector<Plane *> planes,
						BVH *root,
						const int s_samples)
{
	if (!depth)
		return Vec();

	Vec color;
	Material *m = NULL;
	Intersection it, tmp;
	if (root) {
		if (root->intersect(r.ori, r.dir, it))
			m = objects[it.id]->mat;
		else
			it.t = DBL_MAX;
	} else {
		for (unsigned int i = 0; i < objects.size(); ++i) {
			if (objects[i]->intersect(r.ori, r.dir, tmp)) {
				if ((tmp.t > EPSILON) &&
						(tmp.t < it.t)) {
					it = tmp;
					m = objects[i]->mat;
				}
			}
		}
	}

	for (unsigned int i = 0; i < planes.size(); ++i) {
		if (planes[i]->intersect(r.ori, r.dir, tmp)) {
			if ((tmp.t > EPSILON) && (tmp.t < it.t)) {
				it = tmp;
				m = planes[i]->mat;
			}
		}
	}

	if (!m)
		return color;

	Vec p1 = it.point;
	Vec n = it.normal;	/* Normal at intersection */
	Vec ie = r.dir*-1;

	for (unsigned int i = 0; i < lights.size(); ++i) {
		char type = lights[i]->type;

		/* Ambient light: only calculate once */
		if (type == 'a') {
			if (depth == MAX_DEPTH)
				color += lights[i]->rgb*m->diff;
			continue;
		}

		/* Blinn-Phong Shading */
		Ray s_ray;
		s_ray.ori = p1;
		Vec il;
		double max_t;

		/* Square light: v, n, u are corners */
		if (type == 's') {
			Vec sp, tmp;
			for (int j = 0; j < s_samples; ++j) {
				double r1 = (rand()%100 - 50)/100.;
				double r2 = (rand()%100 - 50)/100.;
				double len2 = lights[i]->len/2;
				Vec u2 = lights[i]->n.cross(lights[i]->u);
				u2.normalize();
				sp = lights[i]->v + (u2*len2)*r1 +
						(lights[i]->u*len2)*r2;
				il = sp - p1;

				max_t = il.get_mag() + 1;
				il.normalize();
				s_ray.dir = il;
				Vec irr = shading(s_ray, ie, n,	il, it,	max_t - 1,
						objects, lights[i], m,
						root);

				if (irr.get_mag() < EPSILON)
					continue;

//				irr /= ((max_t+1)*(max_t+1));
				irr *= lights[i]->n.dot(il*-1)/(max_t*max_t);
				tmp += irr;
			}

			color += tmp/s_samples;
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

		color += shading(s_ray, ie, n, il, it, max_t, objects,
						lights[i], m, root);
	}

	/* Ideal specular Shading */
	Vec km = m->ideal;
	if (km.get_mag() > 0.0) {

		/* Reflection ray */
		Vec rfl = n*(2*(n.dot(ie))) - ie;
		rfl.normalize();
		color += km*ray_color(Ray(p1, rfl), depth-1, lights, objects,
							planes, root, 1);
	}

	return color;
}

void Camera::write_exr(const char *outfile)
{
	if (outfile) {
		RgbaOutputFile file(outfile, pw, ph, WRITE_RGBA);
		file.setFrameBuffer(&pixels[0][0], 1, pw);
		file.writePixels(ph);
	}
}
