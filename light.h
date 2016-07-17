/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 */

#ifndef _LIGHT_H
#define _LIGHT_H

#include "ray.h"

class Light {
        public:
        Light(	char t = 'p',
		const Vec &v1 = Vec(),
		const Vec &color = Vec(),
		const Vec &v2 = Vec(),
		const Vec &v3 = Vec(),
		const double &l = 0)
	{ type = t; v = v1; rgb = color; n = v2; u = v3; len = l; }

	~Light(void) {};

	Vec get_rgb(void) const {return rgb;}

        Vec v, rgb, n, u; /* note: n, u, and len used only for area lights */
	double len;
        char type;
};

#endif /* _LIGHT_H */
