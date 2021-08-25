#include "Triangle.h"

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	// find two gradients of the two triangle lines
	float inv_slope_1 = (float)(x1 - x0) / (float)abs((y1 - y0));
	float inv_slope_2 = (float)(x2 - x0) / (float)abs((y2 - y0));

	float x_start = x0;
	float x_end = x0;

	for (int y = y0; y <= y2; y++) {
		draw_line(x_start, y, x_end, y, &color);
		x_start += inv_slope_1;
		x_end += inv_slope_2;
	}
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	// find two gradients of the two triangle lines
	float inv_slope_1 = (float)(x2 - x0) / (float)abs((y2 - y0));
	float inv_slope_2 = (float)(x2 - x1) / (float)abs((y2 - y1));

	float x_start = x2;
	float x_end = x2;

	for (int y = y2; y >= y0; y--) {
		draw_line(x_start, y, x_end, y, &color);
		x_start -= inv_slope_1;
		x_end -= inv_slope_2;
	}
}
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	// sort vertices by y-coord ascending (y0 < y1 < y2)
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}

	if (y1 == y2) {
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else if (y0 == y1) {
		fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else {

		// calculate mid point of the triangle using the triangle similarty
		int My = y1;
		int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

		// draw flat-bottom triangle
		fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);
		// 
		// draw flat-top triangle
		fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);

	}
}

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {
	vec2_t ab = vec2_subtract(b, a);
	vec2_t bc = vec2_subtract(c, b);
	vec2_t ac = vec2_subtract(c, a);
	vec2_t ap = vec2_subtract(p, a);
	vec2_t bp = vec2_subtract(p, b);

	float triangle_area_abc = (ab.x * ac.y - ab.y * ac.x);

	float alpha = (bc.x * bp.y - bc.y * bp.x) / triangle_area_abc;
	float beta = (ap.x * ac.y - ap.y * ac.x) / triangle_area_abc;

	float gamma = 1 - alpha - beta;

	vec3_t weights = { alpha, beta, gamma };
	
	return weights;
}

//	Draw textured pixel at position x and y of the screen
void draw_texel(int x, int y, uint32_t* texture, vec2_t point_a,
	vec2_t point_b, vec2_t point_c, float u0, float v0, float u1, float v1,
	float u2, float v2) {
	
	vec2_t point_p = { x, y };
	vec3_t weights = barycentric_weights(point_a, point_b, point_c, point_p);
	
	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	float interpolated_u = (u0 * alpha) + (u1 * beta) + (u2 * gamma);
	float interpolated_v = (v0 * alpha) + (v1 * beta) + (v2 * gamma);

	int tex_x = abs((int)(interpolated_u * texture_width));
	int tex_y = abs((int)(interpolated_v * texture_height));

	uint32_t color = texture[(texture_width * tex_y) + tex_x];
	draw_pixel(x, y, color);
}

void draw_textured_triangle(
	int x0, int y0, float u0, float v0,
	int x1, int y1, float u1, float v1,
	int x2, int y2, float u2, float v2,
	uint32_t * texture
) {
	// sort vertices by y-coord ascending (y0 < y1 < y2)
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	// create vector points after sorting
	vec2_t point_a = { x0, y0 };
	vec2_t point_b = { x1, y1 };
	vec2_t point_c = { x2, y2 };

	// Render the flat-bottom part of the triangle
	float inv_slope_1 = 0;
	float inv_slope_2 = 0;

	if (y1 - y0 != 0)
		inv_slope_1 = (float)(x1 - x0) / (float)abs((y1 - y0));
	if (y2 - y0 != 0)
		inv_slope_2 = (float)(x2 - x0) / (float)abs((y2 - y0));

	if (y1 - y0 != 0) {
		for (int y = y0; y <= y1; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start)
				int_swap(&x_start, &x_end);

			for (int x = x_start; x <= x_end; x++) {
				//draw_pixel(x, y, &ORANGE);
				draw_texel(
					x, y, texture,
					point_a, point_b, point_c,
					u0, v0, u1, v1, u2, v2
				);
			}
		}
	}

	// Render the flat-top part of the triangle
	inv_slope_1 = 0;
	inv_slope_2 = 0;

	if (y2 - y1 != 0)
		inv_slope_1 = (float)(x2 - x1) / (float)abs((y2 - y1));
	if (y2 - y0 != 0)
		inv_slope_2 = (float)(x2 - x0) / (float)abs((y2 - y0));

	if (y2 - y1 != 0) {
		for (int y = y1; y <= y2; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start)
				int_swap(&x_start, &x_end);

			for (int x = x_start; x <= x_end; x++) {
				//draw_pixel(x, y, &ORANGE);
				draw_texel(
					x, y, texture,
					point_a, point_b, point_c,
					u0, v0, u1, v1, u2, v2
				);
			}
		}
	}
}

