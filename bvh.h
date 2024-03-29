/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 */

#ifndef _BVH_H
#define _BVH_H

#include <vector>
#include <algorithm>

#include "object.h"

class BVH : public Object {
public:
	BVH(void) { left = right = NULL; }
	~BVH(void);
	BVH(vector<Object*> &objects, int l, int r, int axis);
	void surround(const vector<Object*> &objects, int l, int r);
	int intersect(const Vec &r_ori, const Vec &r_dir, Intersection &it);

	Object *left, *right;
};

void delete_bvh(BVH *root);

#endif /* _BVH_H */
