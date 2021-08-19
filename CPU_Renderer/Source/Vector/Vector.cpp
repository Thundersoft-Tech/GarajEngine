#include <math.h>
#include "Vector.h"


///////////////////////
// 2D Vector functions
//////////////////////

vec2_t vec2_rotate(vec2_t v, float angle) {
	vec2_t rotated_vector = {
		v.x * cos(angle) - v.y * sin(angle),
		v.y * cos(angle) + v.x * sin(angle),
	};
	return rotated_vector;
}

float vec2_length(vec2_t v) {
	float h_squared = (v.x * v.x) + (v.y * v.y);
	return (float)sqrt(h_squared);
}

vec2_t vec2_add(vec2_t v1, vec2_t v2) {
	vec2_t result = { v1.x + v2.x, v1.y + v2.y };
	return result;
}

vec2_t vec2_subtract(vec2_t v1, vec2_t v2) {
	vec2_t result = { v1.x - v2.x, v1.y - v2.y };
	return result;
}

vec2_t vec2_multiply(vec2_t v, float scaler) {
	vec2_t result = { v.x * scaler, v.y * scaler };
	return result;
}

vec2_t vec2_divide(vec2_t v, float scaler) {
	vec2_t result = { v.x / scaler, v.y / scaler };
	return result;
}

///////////////////////
// 3D Vector functions
//////////////////////

vec3_t vec3_rotate_x(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		v.x,
		v.y * cos(angle) - v.z * sin(angle),
		v.z * cos(angle) + v.y * sin(angle)
	};
	return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		v.x * cos(angle) - v.z * sin(angle),
		v.y,
		v.z * cos(angle) + v.x * sin(angle)
	};
	return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		v.x * cos(angle) - v.y * sin(angle),
		v.y * cos(angle) + v.x * sin(angle),
		v.z
	};
	return rotated_vector;
}

float vec3_length(vec3_t v) {
	float h_squared = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	return (float)sqrt(h_squared);
}

vec3_t vec3_add(vec3_t v1, vec3_t v2) {
	vec3_t result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return result;
}

vec3_t vec3_subtract(vec3_t v1, vec3_t v2) {
	vec3_t result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return result;
}

vec3_t vec3_multiply(vec3_t v, float scaler) {
	vec3_t result = { v.x * scaler, v.y * scaler, v.z * scaler };
	return result;
}

vec3_t vec3_divide(vec3_t v, float scaler) {
	vec3_t result = { v.x / scaler, v.y / scaler, v.z / scaler };
	return result;
}