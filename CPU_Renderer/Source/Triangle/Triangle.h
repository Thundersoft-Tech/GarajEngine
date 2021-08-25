#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

#include "../Vector/Vector.h"
#include "../Display/Display.h"
#include "../Texture/Texture.h"
#include "../Swap/Swap.h"
#include "../Colors/Colors.h"

typedef struct {
	int a;
	int b;
	int c;
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;
	uint32_t color;
} face_t;

typedef struct {
	vec2_t points[3];
	tex2_t texcoords[3];
	uint32_t color;
	float average_depth;
} triangle_t;

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_textured_triangle(
	int x0, int y0, float u0, float v0,
	int x1, int y1, float u1, float v1,
	int x2, int y2, float u2, float v2,
	uint32_t * texture
);


#endif