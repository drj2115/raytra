#include "material.h"

Material::Material(void)
{
	diff = spec = ideal = attenuate = Vec(0.0, 0.0, 0.0);
	phong = refr_idx = 0.0;
}

Material::Material(	double dr, double dg, double db,
			double sr, double sg, double sb,
			double r,
			double ir, double ig, double ib)
{
	diff = Vec(dr, dg, db);
	spec = Vec(sr, sg, sb);
	ideal = Vec(ir, ig, ib);
	phong = r;
}

void Material::set_refr(double refr_index, double ar, double ag, double ab)
{
	refr_idx = refr_index;
	attenuate = Vec(ar, ag, ab);
}

Vec Material::phong_shading(const Vec &ie, const Vec &n, const Vec &il, const Vec &lRgb)
{
	Vec h = ie + il;
	h.normalize();

	double cosnl = 0.0 > n.dot(il) ? 0.0 : n.dot(il);
	double cosnh = 0.0 > n.dot(h) ? 0.0 : n.dot(h);

	return (diff*lRgb)*cosnl + (spec*lRgb)*pow(cosnh, phong);
}
