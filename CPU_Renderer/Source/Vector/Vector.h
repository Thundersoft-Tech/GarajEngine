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
vec2_t vec2_rotate(vec2_t v);

///////////////////////
// 3D Vector functions
//////////////////////

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);
float vec3_length(vec3_t v);

#endif
