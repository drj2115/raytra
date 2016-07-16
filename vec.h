/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 *
 * Modified version of Qi Wang's Raytra Vector class.
 * Copyright © 2016 Wang Qi. All rights reserved.
 */

#ifndef _VEC_H
#define _VEC_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

class Vec{
friend class Triangle;
	public:
	Vec(void) {x = y = z = 0;}
	Vec(double x1, double y1, double z1);
	Vec(const Vec &p);
	Vec(const Vec &p, const Vec &q);

	Vec& operator=(const Vec &v);
	Vec operator+(const Vec &v) const;
	Vec& operator+=(const Vec &v);
	Vec operator-(const Vec &v) const;
	Vec operator*(double k) const;
	Vec& operator*=(double k);
	Vec operator/(double k) const;
	Vec& operator/=(double k);

	/* Per element scaling of RGB */
	Vec operator*(const Vec &v) const;

	double dot(const Vec &v) const;
	Vec cross(const Vec &v) const;
	void normalize();
	double get_mag() const;
	void print(const char *str = NULL);

	double x, y, z;
};

inline Vec::Vec(double x1, double y1, double z1){
	x = x1;
	y = y1;
	z = z1;
}

inline Vec& Vec::operator=(const Vec &v){
	x = v.x;
	y = v.y;
	z = v.z;
    return *this;
}

inline void Vec::normalize(){
	double length = get_mag();
	x /= length;
	y /= length;
	z /= length;
}

#endif /* _VEC_H */
