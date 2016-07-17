#include "parse.h"

int token_to_float(const char *in_string, float *arr, int start_idx, int len)
{
        char buf[strlen(in_string) + 1];
        char *token;
        int i;

        if (!in_string || !arr || start_idx < 0 || len < 1) {
                printf("Unable to convert the desired token to a float!\n");
                return -1;
        }

        strcpy(buf, in_string);
        token = strtok(buf, " ");

        for (i = 0; i < start_idx - 1; ++i) {
                if (!(token = strtok(NULL, " \t\r\b"))) {
                        printf("Attempting to read past the end of the "
                                                                "line!\n");
                        return -1;
                }
        }

        for (i = 0; i < len; ++i) {
                if (!(token = strtok(NULL, " \t\r\b"))) {
                        printf("Attempting to read past the end of the "
                                                                "line!\n");
                        return -1;
                }
                arr[i] = atof(token);
        }
        return 0;
}

int read_wavefront(const char *w_file,	vector<int> &tri_ids,
					vector<float> &tri_verts)
{
        tri_ids.clear();
        tri_verts.clear();
        char buf[BUF_SIZE] = {0};
        int ret = -1;

        if (!w_file)
                return ret;

        FILE *file = fopen(w_file, "r");
        if (!file) {
                printf("Unable to open %s.\n", w_file);
                return ret;
        }
        while(fgets(buf, sizeof(buf), file) != NULL) {
                char *token = strtok(buf, " ");
                if (buf[0] == 'v') {
                        double pa, pb, pc;
                        if (!(token = strtok(NULL, " ")))
                                goto err_out;
                        pa = atof(token);
                        if (!(token = strtok(NULL, " ")))
                                goto err_out;
                        pb = atof(token);
                        if(!(token = strtok(NULL, " ")))
                                goto err_out;
                        pc = atof(token);
                        tri_verts.push_back(pa);
                        tri_verts.push_back(pb);
                        tri_verts.push_back(pc);
                } else if (buf[0] == 'f') {
                        int i, j, k;
                        if (!(token = strtok(NULL, " ")))
                                goto err_out;
                        i = atoi(token);
                        if (!(token = strtok(NULL, " ")))
                                goto err_out;
                        j = atoi(token);
                        if (!(token = strtok(NULL, " ")))
                                goto err_out;
                        k = atoi(token);
                        tri_ids.push_back(i-1);
                        tri_ids.push_back(j-1);
                        tri_ids.push_back(k-1);
                }
        }

        ret = 0;
err_out:
        fclose(file);
        return ret;
}

int parse(char *filename,	vector<Camera *> *cameras,
				vector<Object *> *objects,
				vector<Plane *> *planes,
				vector<Light *> *lights,
				vector<Material *> *materials)
{
        char line[BUF_SIZE] = {0};
        int ret = -1;
        if (!filename)
                return ret;

        FILE *file = fopen(filename, "r");
        if (!file) {
                printf("Unable to open %s.\n", filename);
                return ret;
        }

        Material *mat = NULL;

        while (fgets(line, sizeof(line), file) != NULL) {
                switch (line[0]) {

                case '/':
                        break;

		/* Material: a[]: dr,dg,db,sr,sg,sb,r,ir,ig,ib */
                case 'm': {
                        float a[10] = {0};
                        if (token_to_float(line, &a[0], 1, 10) < 0)
                                goto err_out;
                        Material *m = new Material(a[0], a[1], a[2], a[3], a[4],
                                                a[5], a[6], a[7], a[8], a[9]);
                        mat = m;

			//implement refraction here......

                        materials->push_back(m);
                        break;

		/* Triangle: a[]: x1,y1,z1,x2,y2,z2,x3,y3,z3 */
                } case 't': {
                        float a[9] = {0};
                        if (token_to_float(line, a, 1, 9) < 0)
                                goto err_out;
                        Triangle *t = new Triangle(a[0], a[1], a[2], a[3], a[4],
						a[5], a[6], a[7], a[8], mat);
                        objects->push_back(dynamic_cast<Object *>(t));
                        break;

		/* Sphere: a[]: x,y,z,r */
                } case 's': {
                        float a[4] = {0};
                        if (token_to_float(line, a, 1, 4) < 0)
                                goto err_out;
                        Vec v(a[0], a[1], a[2]);
                        Sphere *s = new Sphere(mat, v, a[3]);
                        objects->push_back(dynamic_cast<Object *>(s));
                        break;

		/* Plane: a[]: x,y,z,d */
                } case 'p': {
                        float a[4] = {0};
                        if (token_to_float(line, a, 1, 4) < 0)
                                goto err_out;
			Plane *p = new Plane(a[0], a[1], a[2], a[3], mat);
			planes->push_back(p);
                        break;

		/* Light */
                } case 'l': {
                        Light *l = NULL;

			/* Ambient light: a[]: x,y,z */
                        if (line[2] == 'a') {
                                float a[3] = {0};
                                if (token_to_float(line, a, 2, 3) < 0)
                                        goto err_out;
				l = new Light('a', Vec(), Vec(a[0], a[1], a[2]));

			/* Square light: a[]: x,y,z,nx,ny,nz,ux,uy,uz,len,r,g,b */
                        } else if (line[2] == 's') {
				float a[13] = {0};
				if (token_to_float(line, a, 2, 13) < 0)
					goto err_out;
				l = new Light('s', Vec(a[0], a[1], a[2]),
						Vec(a[10], a[11], a[12]),
						Vec(a[3], a[4], a[5]),
						Vec(a[6], a[7], a[8]), a[9]);

			/* Point or Directional light: a[]: x,y,z,r,g,b */
			} else {
                                float a[6] = {0};
                                if (token_to_float(line, a, 2, 6) < 0)
                                        goto err_out;
				l = new Light('p', Vec(a[0], a[1], a[2]),
							Vec(a[3], a[4], a[5]));
				if (line[2] == 'd')
					l->type = 'd';
                        }
                        lights->push_back(l);
                        break;

		/* Camera: a[]: x,y,z,vx,vy,vz,d,iw,ih,pw,ph */
                } case 'c': {
                        float a[11] = {0};
                        if (token_to_float(line, a, 1, 11) < 0)
                                goto err_out;
                        Vec v1(a[0], a[1], a[2]);
                        Vec v2(a[3], a[4], a[5]);
			Camera *c = new Camera(v1, v2, a[6], a[7], a[8],
						(int)a[9], (int)a[10]);
                        cameras->push_back(c);
                        break;

		/* Wavefront */
                } case 'w':{
                        char buf[strlen(line) + 1];
                        char *w_file = NULL;
                        strcpy(buf, line);
                        if (!(w_file = strtok(buf, " \t\n\r")))
                                goto err_out;
                        if (!(w_file = strtok(NULL, " \t\n\r")))
                                goto err_out;
                        vector<int> tri_ids;
                        vector<float> tri_verts;
                        if (read_wavefront(w_file, tri_ids, tri_verts) < 0)
                                goto err_out;
			double x1, y1, z1, x2, y2, z2, x3, y3, z3;
                        for (unsigned int k = 0; k < tri_ids.size()/3; ++k) {
                                x1 = tri_verts[3*tri_ids[3*k]];
                                y1 = tri_verts[3*tri_ids[3*k]+1];
                                z1 = tri_verts[3*tri_ids[3*k]+2];
                                x2 = tri_verts[3*tri_ids[3*k+1]];
                                y2 = tri_verts[3*tri_ids[3*k+1]+1],
                                z2 = tri_verts[3*tri_ids[3*k+1]+2];
                                x3 = tri_verts[3*tri_ids[3*k+2]];
                                y3 = tri_verts[3*tri_ids[3*k+2]+1];
                                z3 = tri_verts[3*tri_ids[3*k+2]+2];
				Triangle *t = new Triangle(x1, y1, z1, x2, y2,
							z2, x3, y3, z3, mat);
                                objects->push_back(dynamic_cast<Object *>(t));
                        }
                }
                } /* switch end */

                memset(&line, 0, sizeof(line));
        }

        /* Missing input */
        if (!lights->size() || !cameras->size() ||
					!(objects->size() || planes->size()))
		goto err_out;

        ret = 0;
err_out:
        fclose(file);
        return ret;
}
