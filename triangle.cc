#include "triangle.h"

Triangle::Triangle(const double x1, const double y1, const double z1,
		   const double x2, const double y2, const double z2,
		   const double x3, const double y3, const double z3,
							Material *m)
{
	p1 = Vec(x1, y1, z1);
	p2 = Vec(x2, y2, z2);
	p3 = Vec(x3, y3, z3);

	a = x1 - x2;
	b = y1 - y2;
	c = z1 - z2;
	d = x1 - x3;
	e = y1 - y3;
	f = z1 - z3;

	n = (p2-p1).cross(p3-p1);
	n.normalize();
	mat = m;

	float minX = x1 < x2 ? x1 : x2;
	minX = minX < x3 ? minX : x3;
	float minY = y1 < y2 ? y1 : y2;
	minY = minY < y3 ? minY : y3;
	float minZ = z1 < z2 ? z1 : z2;
	minZ = minZ < z3 ? minZ : z3;

	float maxX = x1 > x2 ? x1 : x2;
	maxX = maxX > x3 ? maxX : x3;
	float maxY = y1 > y2 ? y1 : y2;
	maxY = maxY > y3 ? maxY : y3;
	float maxZ = z1 > z2 ? z1 : z2;
	maxZ = maxZ > z3 ? maxZ : z3;

	min = Vec(minX - EPSILON, minY - EPSILON, minZ - EPSILON);
	max = Vec(maxX + EPSILON, maxY + EPSILON, maxZ + EPSILON);
	bbox = Bbox(min, max, -1);
}

int Triangle::intersect(const Ray &r, Intersection &it, int bboxOnly)
{

	if (!bbox.intersect(r, it))
		return 0;

	if (bboxOnly)
		return 1;

	g = r.dir.x;
	h = r.dir.y;
	i = r.dir.z;

	eihf = e*i - h*f;
	gfdi = g*f - d*i;
	dheg = d*h - e*g;

	M = a*eihf + b*gfdi + c*dheg;

	if (M == 0)
		return 0;

	j = p1.x - r.ori.x;
	k = p1.y - r.ori.y;
	l = p1.z - r.ori.z;

	akjb = a*k - j*b;
	jcal = j*c - a*l;
	blkc = b*l - k*c;

	t = -(f*akjb + e*jcal + d*blkc)/M;
	if(t < 0)
		return 0;

	double gamma = (i*akjb + h*jcal + g*blkc)/M;
	if(gamma < 0 || gamma > 1)
		return 0;

	double beta = (j*eihf + k*gfdi + l*dheg)/M;
	if(beta < 0 || beta > 1 - gamma)
		return 0;

	Vec p = r.ori + r.dir * t;
	it.set(t, p, n);
	return 1;
}
