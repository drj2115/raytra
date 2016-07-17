/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 */

#ifndef _SPHERE_H
#define _SPHERE_H

#include "object.h"

class Sphere: public Object {
	public:
	Sphere(void) { radius = 0; center = Vec(); }
	Sphere(Material *m, const Vec &p, double r);

	int intersect(const Ray &r, Intersection &it, int bboxOnly);
	Vec get_normal(const Vec &p);

	double radius;
	Vec center;
};

#endif /* _SPHERE_H */
