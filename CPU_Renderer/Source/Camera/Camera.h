#ifndef CAMERA_H
#define CAMERA_H

#include "../Vector/Vector.h"

typedef struct {
	vec3_t position;
	vec3_t direction;
} camera_t;

extern camera_t camera;

#endif // !CAMERA_H

