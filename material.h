/*
 * Dallas Jones
 * Columbia University - Computer Graphics
 */

#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "light.h"

using namespace std;

class Material{
	public:
	Material(void);
	Material(double dr, double dg, double db,
			 double sr, double sg, double sb,
			 double r, double ir, double ig, double ib);

	void set_refr(double ref_index, double ar, double ag, double ab);
	Vec phong_shading(const Vec &ie, const Vec &n,
						const Vec &il, const Vec &lrgb);
	Vec diff, spec, ideal, attenuate;
	double phong, refr_idx;	/* Phong exponent and refraction index */

};

#endif /* _MATERIAL_H */
