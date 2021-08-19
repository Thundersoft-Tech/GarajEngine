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