#include "plane.h"

Plane::Plane(double nx, double ny, double nz,
			 double distance, Material *m){
	n = Vec(nx, ny, nz);
	n.normalize();
	d = distance;
	mat = m;
}

int Plane::intersect(const Ray &r, Intersection &it, int bboxOnly){
	double denominator = r.dir.dot(n);
	if (!denominator)
		return 0;

	double numerator = -(r.ori.dot(n) + d);
	double t = numerator / denominator;
	if (t > 0) {
		Vec p = r.ori + r.dir * t;
		it.set(t, p, n);
		return 1;
	}
	return 0;
}
