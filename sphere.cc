#include "sphere.h"

Sphere::Sphere(Material *&m, const Vec &p, double r){
	mat = m;
	center = p;
	radius = r;
	min = center - Vec(r + EPSILON, r + EPSILON, r + EPSILON);
	max = center + Vec(r - EPSILON, r - EPSILON, r - EPSILON);
	bbox = Bbox(min, max, -1);
}

int Sphere::intersect(const Ray &r, Intersection &it, int bboxOnly){

	if(!bbox.intersect(r, it))
		return 0;

	if(bboxOnly)
		return 1;

	Vec v = r.ori - center;
	Vec d = r.dir;

	double tmp1 = d.dot(v);
	double tmp2 = d.dot(d);
	double tmp3 = v.dot(v) - radius*radius;
	double desc = tmp1 * tmp1 - tmp2 * tmp3;

	if(desc >= 0){
		double t1;
		Vec dneg = d * (-1);
		if(desc == 0)
			t1 = dneg.dot(v)/tmp2;
		else
			t1 = (dneg.dot(v) - sqrt(desc))/tmp2;

		Vec p1 = r.ori + r.dir * t1;
		Vec n = get_normal(p1);

		it.set(t1, p1, n);
		return 1;
	}
	return 0;
}

Vec Sphere::get_normal(const Vec &p){
	Vec n = p - center;
	n.normalize();
	return n;
}
