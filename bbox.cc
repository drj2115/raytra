#include "bbox.h"

int Bbox::intersect(const Vec &r_ori, const Vec &r_dir, Intersection &it)
{
	double dx = 1/r_dir.x;
	double dy = 1/r_dir.y;

	double txmin = (min_p.x - r_ori.x) * dx;
	double txmax = (max_p.x - r_ori.x) * dx;

	if(dx < 0)
		std::swap(txmin, txmax);

	double tymin = (min_p.y - r_ori.y) * dy;
	double tymax = (max_p.y - r_ori.y) * dy;

	if(dy < 0)
		std::swap(tymin, tymax);

	if(txmin > tymax || tymin > txmax)
		return 0;

	double tmin = txmin > tymin ? txmin : tymin;
	double tmax = txmax < tymax ? txmax : tymax;

	double dz = 1/r_dir.z;
	double tzmin, tzmax;

	tzmin = (min_p.z - r_ori.z)*dz;
	tzmax = (max_p.z - r_ori.z)*dz;

	if(dz < 0)
		std::swap(tzmin, tzmax);

	if(tmin > tzmax || tzmin > tmax)
		return 0;

	if (tmin < tzmin)
		tmin = tzmin;
	if (tmax > tzmax)
		tmax = tzmax;

	if (tmax > tmin) {
		Vec n;
		if (fabs(txmin - tmin) < EPSILON)
			n = dx > 0? Vec(-1, 0, 0) : Vec(1, 0, 0);
		else if (fabs(tymin - tmin) < EPSILON)
			n = dy > 0? Vec(0, -1, 0) : Vec(0, 1, 0);
		else if (fabs(tzmin - tmin) < EPSILON)
			n = dz > 0? Vec(0, 0, -1) : Vec(0, 0, 1);
		else
			return 0;

		if (bbox_only)
			it.set(tmin, r_ori + r_dir*tmin, n);

		if (id != -1)
			it.id = id;

		return 1;
	}


	return 0;
}

void Bbox::print(char *str1, char *str2)
{
	min_p.print(str1);
	max_p.print(str2);
}
