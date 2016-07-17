/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 */

#ifndef _PLANE_H
#define _PLANE_H

#include "object.h"
#include "vec.h"

class Plane : public Object {
public:
	Plane(void) { n = Vec(); d = 0; }
	Plane(double nx, double ny, double nz, double d, Material *m);
//	int intersect(const Ray &r, Intersection &it);
	int intersect(const Vec &r_ori, const Vec &r_dir, Intersection &it);
	Vec n;
	double d;
};

#endif /* _PLANE_H */
