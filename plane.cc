#include "plane.h"

Plane::Plane(double nx, double ny, double nz,
			 double distance, Material *m){
	n = Vec(nx, ny, nz);
	n.normalize();
	d = distance;
	mat = m;
}

int Plane::intersect(const Vec &r_ori, const Vec &r_dir, Intersection &it)
{
	double denom = r_dir.dot(n);
	if (!denom)
		return 0;

	double numer = -(r_ori.dot(n) + d);
	double t = numer/denom;
	if (t > EPSILON) {
		it.set(t, r_ori + r_dir*t, n);
		return 1;
	}
	return 0;
}
