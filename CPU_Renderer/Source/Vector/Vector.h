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

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vec4_t;

// Function signatures to manipulate vectors

///////////////////////
// Vector 2 functions
//////////////////////

float vec2_length(vec2_t v);
vec2_t vec2_rotate(vec2_t v, float angle);
vec2_t vec2_add(vec2_t v1, vec2_t v2);
vec2_t vec2_subtract(vec2_t v1, vec2_t v2);
vec2_t vec2_multiply(vec2_t v, float scaler);
vec2_t vec2_divide(vec2_t v, float scaler);
float vec2_dot(vec2_t v1, vec2_t v2);
void vec2_normalize(vec2_t* v);

///////////////////////
// Vector 3 functions
//////////////////////

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);
float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_subtract(vec3_t v1, vec3_t v2);
vec3_t vec3_multiply(vec3_t v, float scaler);
vec3_t vec3_divide(vec3_t v, float scaler);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
float vec3_dot(vec3_t v1, vec3_t v2);
void vec3_normalize(vec3_t* v);

////////////////////////////////
// Vector Conversion functions
///////////////////////////////

vec4_t vec4_from_vec3(vec3_t v);
vec3_t vec3_from_vec4(vec4_t v);

#endif
