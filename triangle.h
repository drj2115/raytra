/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 */

#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <stdio.h>
#include "object.h"


class Triangle : public Object{
public:
	Triangle(void) { p1 = p2 = p3 = n = Vec(); a = b = c = d = e = f = 0; }
	Triangle(const Vec &a, const Vec &b, const Vec &c, Material *m);
	Triangle(const double x1, const double y1, const double z1,
		 const double x2, const double y2, const double z2,
		 const double x3, const double y3, const double z3, Material *m);

	int intersect(const Ray &r, Intersection &it, int bboxOnly);

	Vec get_normal(void) { return n; }

	Vec p1, p2, p3, n;
	double a, b, c, d, e, f, g, h, i, j, k, l, eihf, gfdi, dheg, M, akjb,jcal,blkc,t;
};

#endif /* _TRIANGLE_H */
