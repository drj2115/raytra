#include "vec.h"

Vec::Vec(const Vec &p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

Vec::Vec(const Vec &p, const Vec &q)
{
	x = q.x - p.x;
	y = q.y - p.y;
	z = q.z - p.z;
}

Vec Vec::operator+(const Vec &v) const
{
	return Vec(x+v.x, y + v.y, z + v.z);
}

Vec & Vec::operator +=(const Vec &v)
{
	x += v.x;
	y += v.y;
	z += v.z;
    return *this;
}

Vec Vec::operator-(const Vec &v) const
{
	return Vec(x-v.x, y-v.y, z - v.z);
}

Vec Vec::operator*(double k) const
{
	return Vec(x*k, y*k, z*k);
}

Vec& Vec::operator*=(double k)
{
	x *= k;
	y *= k;
	z *= k;
    return *this;
}

Vec Vec::operator*(const Vec &v) const
{
	return Vec(x*v.x, y*v.y, z*v.z);
}

Vec Vec::operator/(double k) const
{
	return Vec(x/k, y/k, z/k);
}

Vec& Vec::operator/=(double k)
{
	x /= k;
	y /= k;
	z /= k;
    return *this;
}

double Vec::dot(const Vec &v) const
{
	return x*v.x + y*v.y + z*v.z;
}

Vec Vec::cross(const Vec &v) const
{
	double x_val = y*v.z - v.y*z;
	double y_val = z*v.x - x*v.z;
	double z_val = x*v.y - y*v.x;
	return Vec(x_val, y_val, z_val);
}

double Vec::get_mag() const
{
	return sqrt(x*x + y*y + z*z);
}

void Vec::print(const char *str)
{
	if (str)
		printf("%s: <%f, %f, %f>", str, x, y, z);
	else
		printf("<%f, %f, %f>", x, y, z);
}
