#include "ray.h"

Ray::Ray(const Vec &a, const Vec &b)
{
        ori = a;
        dir = b;
        dir.normalize();
}

Ray::Ray(const Ray &a)
{
        this->ori = a.ori;
        this->dir = a.dir;
        this->dir.normalize();
}
