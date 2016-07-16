#include "ray.h"

Ray::Ray(const Vec &a, const Vec &b, float c)
{
        ori = a;
        dir = b;
        dir.normalize();
        acc = c;
}

Ray::Ray(const Ray &a)
{
        this->ori = a.ori;
        this->dir = a.dir;
        this->dir.normalize();
        this->acc = a.acc;
}
