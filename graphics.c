#include "graphics.h"

int cmp(const void *x, const void *y) {
	return (int) (*(float *) x - *(float *) y);
}

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
// m  - drawing mode [LINE, FILL]
// x, y - center
// r  - radius
void circle(render_t *re, int m, float x, float y, float r) {
	ellipse(re, m, r, r, x, y);
}

// render polygon
// r - renderer
// m - drawing mode [LINE, FILL]
// v - point list
// n - number of points
void polygon(render_t *r, int m, float *v, int n) {
	switch (m) {
	case LINE:
		for (int i = 0; i < n; i++) {
			line(r, v[2 * i + 0], v[2 * i + 1], v[2 * ((i + 1) % n) + 0], v[2 * ((i + 1) % n) + 1]);
		}
		break;
	case FILL:
		SDL_Vertex *vertices = calloc(3 * n, sizeof(SDL_Vertex));
		for (int i = 0; i < n - 2; i++) {
			SDL_FPoint p[3] = {0};
			SDL_FColor col = {0};
			memcpy(&p[0], v + 0 * (i + 1), 2 * sizeof(float));
			memcpy(&p[1], v + 2 * (i + 1), 4 * sizeof(float));
			SDL_GetRenderDrawColorFloat(r, &col.r, &col.g, &col.b, &col.a);
			for (int j = 0; j < 3; j++)
				vertices[3 * i + j] = (SDL_Vertex) {p[j], col};
		}
		SDL_RenderGeometry(r, NULL, vertices, 3 * (n - 2), NULL, 0);
		break;
	}
}

/*
// render polygon (scanline)
// r - renderer
// m - drawing mode [LINE, FILL]
// v - point list
// n - number of points
void polygon(render_t *r, int m, vec2 *v, int n) {
	if (m == LINE) {
		for (int i = 0; i < n; i++) {
			line(r, v[i][0], v[i][1], v[(i+1)%n][0], v[(i+1)%n][1]);
		}
		return;
	}
	typedef struct { float k, x0, x1, y0, y1, ymin, ymax; } edge;
	float ymin = v[0][1], ymax = v[0][1];
	edge *e = malloc(n * sizeof(edge));
	for (int i = 0; i < n; i++) {
		float x0 = v[i][0];
		float y0 = v[i][1];
		float x1 = v[(i + 1)%n][0];
		float y1 = v[(i + 1)%n][1];
		float eymin = (y0 < y1) ? y0 : y1;
		float eymax = (y0 > y1) ? y0 : y1;
		if (x0 > x1) {
			swap(float, x0, x1);
			swap(float, y0, y1);
		}
		float k = (y1 - y0) / (x1 - x0);
		e[i] = (edge) { k, x0, x1, y0, y1, eymin, eymax };
		if (y0 < ymin) ymin = y0;
		if (y1 < ymin) ymin = y1;
		if (y0 > ymax) ymax = y0;
		if (y1 > ymax) ymax = y1;
	}
	for (float y = ymin; y < ymax; y++) {
		int xn = 0;
		float *xi = malloc(n * sizeof(float));	
		for (int i = 0; i < n; i++) {
			if (e[i].k && y > e[i].ymin && y <= e[i].ymax) {
				xi[xn++] = e[i].x0 + (y - e[i].y0) / e[i].k;
			}
		}
		qsort(xi, xn, sizeof(float), cmp);
		for (int i = 0; i < xn; i += 2) {
			line(r, xi[i], y, xi[i + 1], y);
		}
		free(xi);
	}
	free(e);
}
*/

// render wireframe mesh
// r - renderer
// v - vertices (list of points)
// f - faces (list of triangles)
// nv - number of vertices
// nf - number of faces
// h  - window height
void mesh(render_t *r, float *v, int *f, int nv, int nf, int h) {
	int xmin = (int) v[0], xmax = (int) v[0];
	int ymin = (int) v[1], ymax = (int) v[1];
	for (int i = 0; i < nv; i++) {
		if (v[3 * i + 0] < xmin) xmin = (int) round(v[3 * i + 0]);
		if (v[3 * i + 0] > xmax) xmax = (int) round(v[3 * i + 0]);
		if (v[3 * i + 1] < ymin) ymin = (int) round(v[3 * i + 1]);
		if (v[3 * i + 1] > ymax) ymax = (int) round(v[3 * i + 1]);
	}
	float *proj = malloc(2 * nv * sizeof(float));
	for (int i = 0; i < nv; i++) {
		float x = v[3 * i + 0] / v[3 * i + 2];
		float y = v[3 * i + 1] / v[3 * i + 2];
		proj[2 * i + 0] = 1 - (x - xmin) / (xmax - xmin);
		proj[2 * i + 1] = 	  (y - ymin) / (ymax - ymin);
	}
	for (int i = 0; i < nf; i++) {
		float t[6] = {0};
		for (int j = 0; j < 3; j++) {
			memcpy(&t[2 * j], &proj[2 * f[3 * i + j]], 
				2 * sizeof(float));
			for (int k = 0; k < 2; k++)
				t[2 * j + k] *= h;
		}
		polygon(r, LINE, t, 3);
	}
	free(proj);
}
