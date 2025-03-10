#include "glm.h"

// v = 0
void vec2_zero(vec2 v) {
	for (int i = 0; i < 2; i++) v[i] = 0;
}

// a · b
float vec2_dot(vec2 a, vec2 b) {
	float n = 0;
	for (int i = 0; i < 2; i++)
		n += a[i] * b[i];
	return n;
}

// ||v|| ** 2
float vec2_norm2(vec2 v) {
	return vec2_dot(v, v);
}

// ||v||
float vec2_norm(vec2 v) {
	return (float) sqrt(vec2_norm2(v));
}

// c = a + b
void vec2_add(vec2 a, vec2 b, vec2 c) {
	for (int i = 0; i < 2; i++) c[i] = a[i] + b[i];
}

// c = a - b
void vec2_sub(vec2 a, vec2 b, vec2 c) {
	for (int i = 0; i < 2; i++) c[i] = a[i] - b[i];
}

// v *= s
void vec2_scale(vec2 v, float s) {
	for (int i = 0; i < 2; i++) v[i] *= s;
}

// v /= ||v||
void vec2_normalize(vec2 v) {
	float n = vec2_norm(v);
	vec2_scale(v, 1/n);
}

// d = rot(v, th)
void vec2_rot(vec2 v, float th, vec2 d) {
	mat2 rot = {
		{(float) cos(th), (float) -sin(th)}, 
		{(float) sin(th), (float)  cos(th)}
	};
	mat2_mulv(rot, v, d);
}

// d = ref(v, th)
void vec2_ref(vec2 v, float th, vec2 d) {
	mat2 ref = {
		{(float) cos(2*th), (float)  sin(2*th)},
		{(float) sin(2*th), (float) -cos(2*th)}
	};
	mat2_mulv(ref, v, d);
}

// d = shear(v, s)
void vec2_shear(vec2 v, vec2 s, vec2 d) {
	mat2 shear = {
		{1 + s[0] * s[1], s[0]},
		{s[1], 				1 },
	};
	mat2_mulv(shear, v, d);
}

// m = 0
void mat2_zero(mat2 m) {
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			m[i][j] = 0;
}

// m = I
void mat2_identity(mat2 m) {
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			m[i][j] = (float) (i == j);
}

// c = a * b
void mat2_mul(mat2 a, mat2 b, mat2 c) {
	mat2_zero(c);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			for (int k = 0; k < 2; k++)
				c[i][j] += a[i][k] * b[k][j];
}

// m = T(m)
void mat2_transpose(mat2 m) {
	swap(float, m[0][1], m[1][0]);
}

// d = m * v
void mat2_mulv(mat2 m, vec2 v, vec2 d) {
	int copy = (v == d);
	if (copy) d = malloc(sizeof(vec2));
	vec2_zero(d);
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			d[i] += m[i][j] * v[j];
	if (copy) { memcpy(v, d, sizeof(vec2)); free(d); }
}

// m *= v
void mat2_scale(mat2 m, int v) {
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			m[i][j] *= v;
}

// D(m)
float mat2_det(mat2 m) {
	return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

// v = 0
void vec3_zero(vec3 v) {
	for (int i = 0; i < 3; i++) v[i] = 0;
}

// v = a · b
float vec3_dot(vec3 a, vec3 b) {
	float n = 0;
	for (int i = 0; i < 3; i++)
		n += a[i] * b[i];
	return n;
}

// ||v|| ** 2
float vec3_norm2(vec3 v) {
	return vec3_dot(v, v);
}

// ||v||
float vec3_norm(vec3 v) {
	return (float) sqrt(vec3_norm2(v));
}

// c = a + b
void vec3_add(vec3 a, vec3 b, vec3 c) {
	for (int i = 0; i < 3; i++) c[i] = a[i] + b[i];
}

// c = a - b
void vec3_sub(vec3 a, vec3 b, vec3 c) {
	for (int i = 0; i < 3; i++) c[i] = a[i] - b[i];
}

// v *= s
void vec3_scale(vec3 v, float s) {
	for (int i = 0; i < 3; i++) v[i] *= s;
}

// v /= ||v||
void vec3_normalize(vec3 v) {
	float n = vec3_norm(v);
	vec3_scale(v, 1/n);
}

// d = rotx(v, th)
void vec3_rotx(vec3 v, float th, vec3 d) {
	mat3 rot = {
		{	1,		0,		0	},
		{	0, (float) cos(th), (float) -sin(th)},
		{	0, (float) sin(th), (float)  cos(th)},
	};
	mat3_mulv(rot, v, d);
}

// d = roty(v, th)
void vec3_roty(vec3 v, float th, vec3 d) {
	mat3 rot = {
		{	(float) cos(th),	0,	(float) sin(th)	},
		{		0,		1,		0	},
		{	(float) -sin(th),	0,	(float) cos(th)	},
	};
	mat3_mulv(rot, v, d);
}

// d = rotz(v, th)
void vec3_rotz(vec3 v, float th, vec3 d) {
	mat3 rot = {
		{	(float) cos(th),	(float) -sin(th),	0	},
		{	(float) sin(th),	(float)  cos(th),	0	},
		{		0,			0,		1	},
	};
	mat3_mulv(rot, v, d);
}

// d = shearx(v, s)
void vec3_shearx(vec3 v, vec3 s, vec3 d) {
	mat3 shear = {
		{1, s[1], s[2]},
		{0, 1, 0},
		{0, 0, 1},
	};
	mat3_mulv(shear, v, d);
}

// d = sheary(v, s)
void vec3_sheary(vec3 v, vec3 s, vec3 d) {
	mat3 shear = {
		{1, 0, 0},
		{s[0], 1, s[2]},
		{0, 0, 1},
	};
	mat3_mulv(shear, v, d);
}

// d = shearz(v, s)
void vec3_shearz(vec3 v, vec3 s, vec3 d) {
	mat3 shear = {
		{1, 0, 0},
		{0, 1, 0},
		{s[0], s[1], 1},
	};
	mat3_mulv(shear, v, d);
}

// m = 0
void mat3_zero(mat3 m) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m[i][j] = 0;
}

// m = I
void mat3_identity(mat3 m) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m[i][j] = (float) (i == j);
}

// c = a * b
void mat3_mul(mat3 a, mat3 b, mat3 c) {
	mat3_zero(c);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				c[i][j] += a[i][k] * b[k][j];
}

// m = T(m)
void mat3_transpose(mat3 m) {
	swap(float, m[0][1], m[1][0]);
	swap(float, m[0][2], m[2][0]);
	swap(float, m[1][2], m[2][1]);
}

// d = m * v
void mat3_mulv(mat3 m, vec3 v, vec3 d) {
	int copy = (v == d);
	if (copy) d = malloc(sizeof(vec3));
	vec3_zero(d);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			d[i] += m[i][j] * v[j];
	if (copy) { memcpy(v, d, sizeof(vec3)); free(d); }
}

// m *= s
void mat3_scale(mat3 m, int s) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m[i][j] *= s;
}

// D(m)
float mat3_det(mat3 m) {
	return m[0][0] * m[1][1] * m[2][2] +
		m[0][1] * m[1][2] * m[2][0] +
		m[0][2] * m[1][0] * m[2][1] -
		m[0][2] * m[1][1] * m[2][0] -
		m[0][1] * m[1][0] * m[2][2] -
		m[0][0] * m[1][2] * m[2][1];
}
