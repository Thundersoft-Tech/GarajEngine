#ifndef LIGHT_H
#define LIGHT_H
#include <stdint.h>
#include "../Vector/Vector.h"

typedef struct {
	vec3_t direction;
} light_t;

extern light_t light;

uint32_t apply_light_intensity(uint32_t original_color, float percentage_factor);

#endif // !LIGHT_H
