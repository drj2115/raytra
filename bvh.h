/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 *
 * Modified version of Qi Wang's Raytra BVH class
 * Copyright © 2016 Wang Qi. All rights reservd.
 */

#ifndef _BVH_H
#define _BVH_H

#include <vector>
#include <algorithm>

#include "object.h"

class BVH : public Object{
public:
	Object *left, *right;

	BVH(): left(NULL), right(NULL) {}
	BVH(vector<Object*> &objects, int l, int r, int axis);
	void surround(const vector<Object*> &objects, int l, int r);
	int intersect(const Ray &r, Intersection &it, int bboxOnly);
	int shadow_test(const Ray &r, Intersection &it, int bboxOnly);
};

#endif /* _BVH_H */
