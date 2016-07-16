/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 *
 * Modified version of Qi Wang's Raytra Triangle class.
 * Copyright © 2016 Wang Qi. All rights reserved.
 */

#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <stdio.h>
#include "object.h"

class Triangle : public Object{
public:
	Triangle(void);
	Triangle(const Vec &a, const Vec &b, const Vec &c, Material *&m);
	Triangle(double x1, double y1, double z1,
		 double x2, double y2, double z2,
		 double x3, double y3, double z3, Material *&m);

	virtual int intersect(const Ray &r, Intersection &it, int bboxOnly);

	Vec get_normal(void){return n;}

	Vec p1, p2, p3, n;
	double a, b, c, d, e, f;
};

#endif /* _TRIANGLE_H */
