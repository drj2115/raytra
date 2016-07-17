#ifndef _RAY_H
#define _RAY_H

#include "vec.h"

class Ray {
        public:
        Ray(const Vec &a = Vec(), const Vec &b = Vec());
        Ray(const Ray &a);
        ~Ray(void) {};

        Vec ori, dir;
};

#endif /* #ifndef _RAY_H */
