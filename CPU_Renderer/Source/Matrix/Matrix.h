#ifndef MATRIX_H
#define MATRIX_H
#include "../Vector/Vector.h"
#include <math.h>

typedef struct {
	float m[4][4];
} mat4_t;

// function signatures
mat4_t mat4_identity();

mat4_t mat4_make_rotation_x(float angle);
mat4_t mat4_make_rotation_y(float angle);
mat4_t mat4_make_rotation_z(float angle);

mat4_t mat4_make_translation(float tx, float ty, float tz);
mat4_t mat4_make_scale(float sx, float sy, float sz);

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);
mat4_t mat4_mul_mat4(mat4_t a, mat4_t b);

#endif