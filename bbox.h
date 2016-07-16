#ifndef _BBOX_H
#define _BBOX_H

#include "intersection.h"

#define EPS .001f

class Bbox{
friend class BVH;
public:
	Bbox(void) { id = -1; min_p = Vec(); max_p = Vec(); }

	Bbox(const Vec &minp, const Vec &maxp, int i)
	{
		id = i;
		min_p = minp;
		max_p = maxp;
	}

	int intersect(const Ray &r, Intersection &it);
	void print(char *str1 = NULL, char *str2 = NULL);

	Vec min_p, max_p;
	int id;
};

#endif /* _BBOX_H */
