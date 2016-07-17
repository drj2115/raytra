#include "sphere.h"

Sphere::Sphere(Material *m, const Vec &p, double r)
{
	mat = m;
	center = p;
	radius = r;
	min = center - Vec(r + EPSILON, r + EPSILON, r + EPSILON);
	max = center + Vec(r - EPSILON, r - EPSILON, r - EPSILON);
	bbox = Bbox(min, max, -1);
}

int Sphere::intersect(const Vec &r_ori, const Vec &r_dir, Intersection &it)
{
	if (!bbox.intersect(r_ori, r_dir, it))
		return 0;

	if (bbox_only)
		return 1;

	Vec v = r_ori - center;

	double tmp1 = r_dir.dot(v);
	double tmp2 = r_dir.dot(r_dir);
	double disc = tmp1*tmp1 - tmp2*(v.dot(v) - radius*radius);

	if (disc >= 0) {
		double t1;

		if (!disc)
			t1 = r_dir.dot(v*-1)/tmp2;
		else
			t1 = (r_dir.dot(v*-1) - sqrt(disc))/tmp2;

		Vec p1 = r_ori + r_dir*t1;

		it.set(t1, p1, get_normal(p1));
		return 1;
	}
	return 0;
}

Vec Sphere::get_normal(const Vec &p){
	Vec n = p - center;
	n.normalize();
	return n;
}
