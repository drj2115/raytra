#ifndef _PARSE_H_
#define _PARSE_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "triangle.h"
#include "sphere.h"
#include "plane.h"
#include "cam.h"
#include "light.h"

#define BUF_SIZE 4096

using namespace std;

/*
 * parse:
 * Reads data from the input file and builds the various scene objects, lights,
 * cameras, and materials.
 */

int parse(char *filename,	vector<Camera *> *cameras,
				vector<Object *> *objects,
				vector<Plane *> *planes,
				vector<Light *> *lights,
				vector<Material *> *materials);

/* read_wavefront:
 * Helps parse() parse an .obj, or wavefront, file.
 */
int read_wavefront(const char *w_file,	vector<int> &tri_ids,
					vector<float> &tri_verts);

/*
 * token_to_float:
 * Reads the values from each line in the scene file, translates them from
 * strings to floats, and stores the result in the input array arr.
 * start_idx is the token we want to start on per line (ie a coordinate
 * rather than 'l' or 'c').  len is the length of arr.
 * Returns -1 on error.
 */
int token_to_float(const char *in_string, float *arr, int start_idx, int len);

#endif
