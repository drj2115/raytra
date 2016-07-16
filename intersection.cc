#include "intersection.h"

Intersection::Intersection(void)
{
	t = DBL_MAX;
	point = Vec();
	normal = Vec();
	id = -1;
}

void Intersection::set(double t1, const Vec &p1, const Vec &n)
{
	t = t1;
	point = p1;
	normal = n;
}

Intersection& Intersection::operator=(const Intersection &i)
{
	t = i.t;
	point = i.point;
	normal = i.normal;
	id = i.id;
	return *this;
}

void Intersection::print(char *str1, char *str2)
{
	if (str1)
		printf("%st: %f", str1, t);
	else
		printf("t: %f", t);
	point.print(str2);
}
