/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 */

#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <stdio.h>
#include "object.h"

class Triangle : public Object {
public:
	Triangle(void) { p1 = p2 = p3 = n = Vec(); }
	Triangle(const Vec &a, const Vec &b, const Vec &c, Material *m);
	Triangle(const double x1, const double y1, const double z1,
		 const double x2, const double y2, const double z2,
		 const double x3, const double y3, const double z3, Material *m);

	int intersect(const Vec &r_ori, const Vec &r_dir, Intersection &it);

	Vec get_normal(void) { return n; }

	Vec p1, p2, p3, n, p1_p2, p1_p3, v1, v2;
	double M, t, beta, gamma;
};

#endif /* _TRIANGLE_H */
