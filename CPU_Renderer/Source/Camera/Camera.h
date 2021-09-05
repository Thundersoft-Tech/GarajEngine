#ifndef CAMERA_H
#define CAMERA_H

#include "../Vector/Vector.h"

typedef struct {
	vec3_t position;
	vec3_t direction;
	vec3_t forward_velocity;
	float yaw_angle;
	float pitch_angle;
} camera_t;

extern camera_t camera;

#endif // !CAMERA_H

