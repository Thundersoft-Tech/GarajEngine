#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

// Function signatures to manipulate vectors

///////////////////////
// 2D Vector functions
//////////////////////

float vec2_length(vec2_t v);
vec2_t vec2_rotate(vec2_t v, float angle);
vec2_t vec2_add(vec2_t v1, vec2_t v2);
vec2_t vec2_subtract(vec2_t v1, vec2_t v2);
vec2_t vec2_multiply(vec2_t v, float scaler);
vec2_t vec2_divide(vec2_t v, float scaler);

///////////////////////
// 3D Vector functions
//////////////////////

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);
float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_subtract(vec3_t v1, vec3_t v2);
vec3_t vec3_multiply(vec3_t v, float scaler);
vec3_t vec3_divide(vec3_t v, float scaler);

#endif
