#ifndef _OBJECT_H
#define _OBJECT_H

#include "ray.h"
#include "vec.h"
#include "material.h"
#include "bbox.h"
#include "intersection.h"

class Object {
	public:
	Object(void) { mat = NULL; min = Vec(); max = Vec(); bbox = Bbox(); }

	virtual int intersect(const Vec &r_ori, const Vec &r_dir, Intersection &it)
	{
		return 0;
	}

	Bbox bbox;
	Material *mat;
	Vec min, max;
};

#endif /* _OBJECT_H */
