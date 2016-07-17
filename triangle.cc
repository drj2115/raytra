#include "triangle.h"

Triangle::Triangle(const double x1, const double y1, const double z1,
		   const double x2, const double y2, const double z2,
		   const double x3, const double y3, const double z3,
							Material *m)
{
	double min_x, min_y, min_z, max_x, max_y, max_z;

	p1 = Vec(x1, y1, z1);
	p2 = Vec(x2, y2, z2);
	p3 = Vec(x3, y3, z3);

	p1_p2 = p1 - p2;
	p1_p3 = p1 - p3;

	v1 = v2 = Vec();

	(n = p1_p2.cross(p1_p3)).normalize();
	mat = m;

	min_x = x1 < x2 ? x1 : x2;
	min_x = min_x < x3 ? min_x : x3;
	min_y = y1 < y2 ? y1 : y2;
	min_y = min_y < y3 ? min_y : y3;
	min_z = z1 < z2 ? z1 : z2;
	min_z = min_z < z3 ? min_z : z3;

	max_x = x1 > x2 ? x1 : x2;
	max_x = max_x > x3 ? max_x : x3;
	max_y = y1 > y2 ? y1 : y2;
	max_y = max_y > y3 ? max_y : y3;
	max_z = z1 > z2 ? z1 : z2;
	max_z = max_z > z3 ? max_z : z3;

	min = Vec(min_x - EPSILON, min_y - EPSILON, min_z - EPSILON);
	max = Vec(max_x + EPSILON, max_y + EPSILON, max_z + EPSILON);
	bbox = Bbox(min, max, -1);
}

int Triangle::intersect(const Vec &r_ori, const Vec &r_dir, Intersection &it)
{
	if (!bbox.intersect(r_ori, r_dir, it))
		return 0;

	if (bbox_only)
		return 1;

	v1 = p1_p3.cross(r_dir);

	if (!(M = 1./p1_p2.dot(v1)))
		return 0;

	v2 = p1_p2.cross(p1 - r_ori);

	if ((t = -(v2.dot(p1_p3))*M) < 0)
		return 0;

	if ((gamma = (v2.dot(r_dir))*M) < 0 || gamma > 1)
		return 0;

	if ((beta = ((p1-r_ori).dot(v1))*M) < 0 || (beta > 1 - gamma))
		return 0;

	it.set(t, r_ori + r_dir*t, n);
	return 1;
}
