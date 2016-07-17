/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 */

#ifndef _CAM_H
#define _CAM_H

#include <vector>

#include <ImfRgbaFile.h>
#include <ImfStringAttribute.h>
#include <ImfMatrixAttribute.h>
#include <ImfArray.h>

#include <time.h>
#if defined(_OPENMP)
#	include <omp.h>
#endif

#include "light.h"
#include "bvh.h"
#include "plane.h"

using namespace Imf;
using namespace Imath;
using namespace std;

#define MAX_DEPTH 10

class Camera {
public:
	Camera(void);
	Camera(const Vec &p, const Vec &dir,
		   double d, double iwidth, double iheight,
		   int pwidth, int pheight);

	void set_uvw(const Vec &d);

	Ray gen_ray(const double i, const double j);
	Ray jitter(const double i, const double j, const int p, const int q, const int samples);

	void render(vector<Object *> &objects, vector<Plane *> &planes,
						const vector<Light *> &lights,
						BVH *root,
						const int p_samples,
						const int s_samples,
						const int bbox_flag);

	Vec ray_color(const Ray &r, int depth, const vector<Light *> lights,
						const vector<Object *> objects,
						const vector<Plane *> planes,
						BVH *root,
						const int s_samples,
						const int flag);

	void set_pixel(const int x, const int y, const double r, const double g, const double b)
	{
		Rgba &px = pixels[y][x];
		px.r = r; px.g = g; px.b = b;
	}

	void write_exr(const char *file_name);

private:
	Vec eye, u, v, w;
	double f_len, iw, ih;
	int pw, ph;
	Array2D<Rgba> pixels;
};

#endif /* _CAM_H */
