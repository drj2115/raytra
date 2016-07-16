/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 *
 * Modified version of Qi Wang's Raytra Camera class.
 * Copyright © 2016 Wang Qi. All rights reserved.
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

class Camera{
	public:
	Camera(void);
	Camera(const Vec &p, const Vec &dir,
		   double d, double iwidth, double iheight,
		   int pwidth, int pheight);

	void set_uvw(const Vec &d);

	Ray gen_ray(double i, double j);
	Ray jitter(double i, double j, int p, int q, int samples);

	void render(vector<Object *> &objects, vector<Plane *> &planes,
						const vector<Light *> &lights,
						BVH *root,
						int &p_samples,
						int &s_samples,
						int &bbox_flag);

	Vec ray_color(const Ray &r, int depth, const vector<Light *> lights,
						const vector<Object *> objects,
						const vector<Plane *> planes,
						BVH *root,
						int p_samples,
						int s_samples,
						int &flag);

	void set_pixel(int x, int y, double r, double g, double b)
	{
		Rgba &px = pixels[y][x];
		px.r = r; px.g = g; px.b = b;
	}

	void write_exr(const char *file_name);

	Vec eye, u, v, w;
	double f_len, iw, ih;
	int pw, ph;
	Array2D<Rgba> pixels;
};

#endif /* _CAM_H */
