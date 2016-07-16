#include "bbox.h"

void swap(double &a, double &b)
{
	double temp = a;
	a = b;
	b = temp;
}

int Bbox::intersect(const Ray &ray, Intersection &it){
	double dx = 1/ray.dir.x;
	double dy = 1/ray.dir.y;

	double txmin = (min_p.x - ray.ori.x) * dx;
	double txmax = (max_p.x - ray.ori.x) * dx;

	if(dx < 0)
		swap(txmin, txmax);

	double tymin = (min_p.y - ray.ori.y) * dy;
	double tymax = (max_p.y - ray.ori.y) * dy;

	if(dy < 0)
		swap(tymin, tymax);

	if(txmin > tymax || tymin > txmax)
		return 0;

	double tmin = txmin > tymin ? txmin : tymin;
	double tmax = txmax < tymax ? txmax : tymax;

	double dz = 1/ray.dir.z;
	double tzmin, tzmax;

	tzmin = (min_p.z - ray.ori.z) * dz;
	tzmax = (max_p.z - ray.ori.z) * dz;

	if(dz < 0)
		swap(tzmin, tzmax);

	if(tmin > tzmax || tzmin > tmax)
		return 0;

	if (tmin < tzmin)
		tmin = tzmin;
	if (tmax > tzmax)
		tmax = tzmax;

	if (tmax > tmin) {
		Vec n;
		if(tmin - txmin > -EPS && tmin - txmin < EPS)
			n = dx > 0? Vec(-1, 0, 0) : Vec(1, 0, 0);
		else if(tmin - tymin > -EPS && tmin - tymin < EPS)
			n = dy > 0? Vec(0, -1, 0) : Vec(0, 1, 0);
		else if(tmin - tzmin > -EPS && tmin - tzmin < EPS)
			n = dz > 0? Vec(0, 0, -1) : Vec(0, 0, 1);
		else
			return 0;

		it.set(tmin, ray.ori + ray.dir*tmin, n);
		if(id != -1)
			it.id = id;

		return 1;
	}
	return 0;
}

void Bbox::print(char *str1, char *str2){
	min_p.print(str1);
	max_p.print(str2);
}
