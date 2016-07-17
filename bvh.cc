#include "bvh.h"

inline int cmp_x(Object *a, Object *b) {
	return a->bbox.max_p.x < b->bbox.max_p.x;
}

inline int cmp_y(Object *a, Object *b) {
	return a->bbox.max_p.y < b->bbox.max_p.y;
}

inline int cmp_z(Object *a, Object *b) {
	return a->bbox.max_p.z < b->bbox.max_p.z;
}

BVH::BVH(vector<Object*> &objects, int l, int r, int axis) {
	if (l == r) {
		left = objects[l];
		left->bbox.id = l;
		right = NULL;
	} else if (l < r) {
		surround(objects, l, r);
		if (r == l + 1) {
			left = objects[l];
			left->bbox.id = l;
			right = objects[r];
			right->bbox.id = r;
		} else {
			int mid = l + (r - l) / 2;
			if (axis == 0) {
				nth_element(	objects.begin() + l,
						objects.begin() + mid,
						objects.begin() + r + 1, cmp_x);
			} else if (axis == 1) {
				nth_element(	objects.begin() + l,
						objects.begin() + mid,
						objects.begin() + r + 1, cmp_y);
			} else if (axis == 2) {
				nth_element(	objects.begin() + l,
						objects.begin() + mid,
						objects.begin() + r + 1, cmp_z);
			}
			left = new BVH(objects, l, mid, (axis + 1) % 3);
			right = new BVH(objects, mid + 1, r, (axis + 1) % 3);
		}
	}
}

void BVH::surround(const vector<Object*> &objects, int l, int r) {
	double minx, miny, minz;
	double maxx, maxy, maxz;

	minx = miny = minz = DBL_MAX;
	maxx = maxy = maxz = -DBL_MAX;

	for(int i = l; i <= r; ++i) {
		minx = minx < objects[i]->min.x ? minx : objects[i]->min.x;
		miny = miny < objects[i]->min.y ? miny : objects[i]->min.y;
		minz = minz < objects[i]->min.z ? minz : objects[i]->min.z;
		maxx = maxx > objects[i]->max.x ? maxx : objects[i]->max.x;
		maxy = maxy > objects[i]->max.y ? maxy : objects[i]->max.y;
		maxz = maxz > objects[i]->max.z ? maxz : objects[i]->max.z;
	}

	bbox = Bbox(Vec(minx - EPSILON, miny - EPSILON, minz - EPSILON),
		Vec(maxx + EPSILON, maxy + EPSILON, maxz + EPSILON), -1);
}

int BVH::intersect(const Ray &r, Intersection &it, int bboxOnly) {
	if (!right)
		return (left->intersect(r, it, bboxOnly) && (it.t > 0.0001));

	if (!bbox.intersect(r, it))
		return 0;

	Intersection lIt, rIt;
	int lHit = left->intersect(r, lIt, bboxOnly) && lIt.t > 0.0001;
	int rHit = right->intersect(r, rIt, bboxOnly) && rIt.t > 0.0001;

	if (!lHit && !rHit)
		return 0;

	if (lHit && rHit)
		it = lIt.t < rIt.t ? lIt : rIt;
	else if (lHit)
		it = lIt;
	else
		it = rIt;

	return 1;
}

int BVH::shadow_test(const Ray &r, Intersection &it, int bboxOnly) {
	if (!right)
		return (left->intersect(r, it, bboxOnly) && (it.t > 0.0001));
	if (!bbox.intersect(r, it))
		return 0;
	if (left->intersect(r, it, bboxOnly) && (it.t > 0.0001))
		return 1;
	return (right->intersect(r, it, bboxOnly) && (it.t > 0.0001));
}
