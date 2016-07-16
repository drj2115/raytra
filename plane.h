/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 *
 * Modified version of Qi Wang's Raytra Plane class.
 * Copyright © 2016 Wang Qi. All rights reserved.
 */

#ifndef _PLANE_H
#define _PLANE_H

#include "object.h"
#include "vec.h"

class Plane : public Object {
public:
	Plane(): n(Vec()), d(0.0) {}
	Plane(double nx, double ny, double nz, double d, Material *&m);
	int intersect(const Ray &r, Intersection &it, int bboxOnly);
	Vec n;
	double d;
};

#endif /* _PLANE_H */
