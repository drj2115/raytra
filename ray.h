#ifndef _RAY_H
#define _RAY_H

#include "vec.h"

class Ray {
        public:
        Ray(const Vec &a = Vec(), const Vec &b = Vec(), float c = .00001);
        Ray(const Ray &a);
        ~Ray(void) {};

        Vec ori, dir;
        float acc;
};

#endif /* #ifndef _RAY_H */