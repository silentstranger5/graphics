#include "graphics.h"

// render ellipse
// r - renderer
// m - drawing mode [LINE, FILL]
// a - semi-major axis
// b - semi-minor axis
// h - horizontal shift
// k - vertical shift
void ellipse(render_t *r, int m, float a, float b, float h, float k) {
	switch (m) {
	case LINE:
		for (float x = -a; x < a; x += 0.2f) {
			float y = b * (float) sqrt(1 - (x * x) / (a * a));
			point(r, x + h, -y + k);
			point(r, x + h,  y + k);
		}
		break;
	case FILL:
		for (float y = -b; y < b; y++) {
			float x = a * (float) sqrt(1 - (y * y) / (b * b));
			line(r, -x + h, y + k, x + h, y + k);
		}
		break;
	}
}

// render circle
// re - renderer
// m  - drawing mode
// x, y - center
// r  - radius
void circle(render_t *re, int m, float x, float y, float r) {
	ellipse(re, m, r, r, x, y);
}

int cmpy(const void *x, const void *y) {
    return (int) (((float *) x)[1] - ((float *) y)[1]);
}

// render triangle
// r - renderer
// m - drawing mode
// v - vertex list
void triangle(render_t *r, int m, float *v) {
    switch(m) {
        case LINE:
            for (int i = 0; i < 3; i++) {
                line(r, v[2 * i + 0], v[2 * i + 1],
                    v[2 * ((i+1)%3) + 0], v[2 * ((i+1)%3) + 1]);
            }
            break;
        case FILL:
            qsort(v, 3, 2 * sizeof(float), cmpy);
            for (float y = v[1]; y < v[5]; y++) {
                float x0 = (y < v[3]) ?
                    v[0] + (v[2] - v[0]) * (y - v[1]) / (v[3] - v[1]) :
                    v[2] + (v[4] - v[2]) * (y - v[3]) / (v[5] - v[3]) ;
                float x1 = v[0] + (v[4] - v[0]) * (y - v[1]) / (v[5] - v[1]);
                line(r, x0, y, x1, y);
            }
            break;
    }
}

// render polygon (triangle fan)
// r - renderer
// m - drawing mode
// v - vertex list
// n - number of vertices
void polygon(render_t *r, int m, float *v, int n) {
    for (int i = 0; i < n - 2; i++) {
        triangle(r, m, (float []) {
            v[0], v[1],
            v[2 * (i+1) + 0], v[2 * (i+1) + 1],
            v[2 * (i+2) + 0], v[2 * (i+2) + 1],
        });
    }
}

// int cmp(const void *x, const void *y) {
// 	return (int) (*(float *) x - *(float *) y);
// }

// render polygon (scanline)
// r - renderer
// m - drawing mode
// v - point list
// n - number of points
// void polygon(render_t *r, int m, float *v, int n) {
// 	if (m == LINE) {
// 		for (int i = 0; i < n; i++) {
// 			line(r, v[2 * i + 0], v[2 * i + 1], 
// 				v[2 * ((i+1) % n) + 0], v[2 * ((i+1) % n) + 1]);
// 		}
// 		return;
// 	}
// 	float ymin = v[0], ymax = v[1];
//     float *xi = malloc(n * sizeof(float));
//     for (int i = 0; i < n; i++) {
//         float y = v[2 * i + 1];
//         if (y < ymin) ymin = y;
//         if (y > ymax) ymax = y;
//     }
//     for (float y = ymin; y < ymax; y++) {
//         int xn = 0;
//         for (int i = 0; i < n; i++) {
//             float x0 = v[2 * i + 0];
//             float y0 = v[2 * i + 1];
//             float x1 = v[2 * ((i+1)%n) + 0];
//             float y1 = v[2 * ((i+1)%n) + 1];
//             float ymin = (y0 < y1) ? y0 : y1;
//             float ymax = (y0 > y1) ? y0 : y1;
//             if ((y1 - y0) != 0 && ymin < y && y <= ymax) {
//                 xi[xn++] = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
//             }
//         }
//         qsort(xi, xn, sizeof(float), cmp);
//         for (int i = 0; i < xn; i += 2) {
//             line(r, xi[i], y, xi[i + 1], y);
//         }
//     }
//     free(xi);
// }

// render wireframe mesh
// r - renderer
// v - vertices (list of points)
// f - faces (list of triangles)
// nv - number of vertices
// nf - number of faces
// ch - canvas height
// wh - window height
void mesh(render_t *r, float *v, int *f, int nv, int nf, float ch, float wh) {
	float *proj = malloc(2 * nv * sizeof(float));
	for (int i = 0; i < nv; i++) {
		float x = v[3 * i + 0] / v[3 * i + 2];
		float y = v[3 * i + 1] / v[3 * i + 2];
		proj[2 * i + 0] =	  (x + ch) / (2 * ch);
		proj[2 * i + 1] = 1 - (y + ch) / (2 * ch);
	}
	float t[6] = {0};
	for (int i = 0; i < nf; i++) {
		for (int j = 0; j < 3; j++) {
			memcpy(&t[2 * j], &proj[2 * f[3 * i + j]], 
				2 * sizeof(float));
			for (int k = 0; k < 2; k++)
				t[2 * j + k] *= wh;
		}
		triangle(r, LINE, t);
	}
	free(proj);
}
