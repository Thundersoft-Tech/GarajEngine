#ifndef MATRIX_H
#define MATRIX_H
#include "../Vector/Vector.h"

typedef struct {
	float m[4][4];
} mat4_t;

// function signatures
mat4_t mat4_identity();

mat4_t mat4_make_scale(float sx, float sy, float sz);
mat4_t mat4_make_translation(float tx, float ty, float tz);

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);

#endif