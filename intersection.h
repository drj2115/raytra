/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 *
 * Modified version of Qi Wang's Raytra Intersection class.
 * Copyright © 2016 Wang Qi. All rights reserved.
 */

#ifndef _INTERSECTION_H
#define _INTERSECTION_H

#include <float.h>

#include "ray.h"
#include "material.h"

class Intersection{
public:
	Intersection(void);
	void set(double t1, const Vec &p1, const Vec &n);
	Intersection & operator =(const Intersection &i);
	void print(char *str1 = NULL, char *str2 = NULL);

	double t;
	Vec point, normal;
	int id;
};

#endif /* _INTERSECTION_H */
