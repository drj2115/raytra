#include <stdlib.h>
#include <vector>
//#if defined(_OPENMP)
//#	include <omp.h>
//#endif

#include "light.h"
#include "object.h"
#include "cam.h"
#include "parse.h"

void deleteTree(BVH *root)
{
	if (root) {
		deleteTree(dynamic_cast<BVH *>(root->left));
		deleteTree(dynamic_cast<BVH *>(root->right));
		delete root;
	}
}

int main(int argc, char * argv[]) {
	vector<Light *> lights;
	vector<Object *> objects;
	vector<Plane *> planes;
	vector<Camera *> cameras;
	vector<Material *> materials;
#if defined(_OPENMP)
	double start_time = omp_get_wtime();
#else
	clock_t start_time = clock();
#endif
	BVH *root = NULL;
	int p_samples = 1, s_samples = 1, flag = 3;

	if (argc < 3) {
		printf("Usage: ./prog_out <input file> <output file> "
			"<primary rays> <shadow rays> <flag>\n");
		return -1;
	}

	if (argc > 3) {
		p_samples = atoi(argv[3]);
		if (argc > 4)
			s_samples = atoi(argv[4]);
		if (argc > 5)
			flag = atoi(argv[5]);
		p_samples = p_samples < 1 ? 1 : p_samples;
		s_samples = s_samples < 1 ? 1 : s_samples;
		flag = flag < 0 ? 0 : flag;
	}

	if (parse(argv[1], &cameras, &objects, &planes, &lights, &materials) < 0) {
		printf("Scene file parsing error.\n");
		return -1;
	}

	printf("%u cameras, %u objects, %u planes, %u lights, and %u materials.\n"
		"Rendering 0%%", cameras.size(), objects.size(), planes.size(),
		lights.size(), materials.size());

	fflush(stdout);

	if (flag == 2 || flag == 3) {
		root = new BVH(objects, 0, (int)objects.size()-1, 0);
		flag = (flag == 3) ? 0 : 1;
	}

	Camera *scene_cam = cameras[0];

	scene_cam->render(objects, planes, lights, root, p_samples, s_samples, flag);
	scene_cam->write_exr(argv[2]);

	delete scene_cam;
	deleteTree(root);

	for (unsigned int i = 0; i < materials.size(); ++i)
		delete materials[i];

	for (unsigned int i = 0; i < objects.size(); ++i)
		delete objects[i];

	for (unsigned int i = 0; i < planes.size(); ++i)
		delete planes[i];

	for (unsigned int i = 0; i < lights.size(); ++i)
		delete lights[i];

#if defined(_OPENMP)
	printf(" done!\nRender time: %.2fs\n",
			omp_get_wtime()-start_time);
#else
	printf(" done!\nRender time: %.2fs\n",
			(float)(clock()-start_time)/CLOCKS_PER_SEC);
#endif
	return 0;
}
