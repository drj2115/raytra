#include "triangle.h"

Triangle::Triangle(void)
{
	p1 = p2 = p3 = n = Vec();
	a = b = c = d = e = f = 0.0;
}

Triangle::Triangle(double x1, double y1, double z1,
		   double x2, double y2, double z2,
		   double x3, double y3, double z3, Material *&m)
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

	double g = r.dir.x;
	double h = r.dir.y;
	double i = r.dir.z;

	double eihf = e * i - h * f;
	double gfdi = g * f - d * i;
	double dheg = d * h - e * g;

	double M = a * eihf + b * gfdi + c * dheg;

	if (M == 0)
		return 0;

	double j = p1.x - r.ori.x;
	double k = p1.y - r.ori.y;
	double l = p1.z - r.ori.z;

	double akjb = a * k - j * b;
	double jcal = j * c - a * l;
	double blkc = b * l - k * c;

	double t = -(f * akjb + e * jcal + d * blkc)/M;
	if(t < 0)
		return 0;

	double gamma = (i * akjb + h * jcal + g * blkc)/M;
	if(gamma < 0 || gamma > 1)
		return 0;

	double beta = (j * eihf + k * gfdi + l * dheg)/M;
	if(beta < 0 || beta > 1 - gamma)
		return 0;

	Vec p = r.ori + r.dir * t;
	it.set(t, p, n);
	return 1;
}
