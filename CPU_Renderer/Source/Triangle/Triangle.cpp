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
void draw_texel(int x, int y, uint32_t* texture, vec4_t point_a,
	vec4_t point_b, vec4_t point_c,
	float a_w_reciprocal, float b_w_reciprocal, float c_w_reciprocal,
	float u0, float v0, float u1, float v1,
	float u2, float v2) {
	
	vec2_t point_p = { x, y };
	vec2_t a = vec2_from_vec4(point_a);
	vec2_t b = vec2_from_vec4(point_b);
	vec2_t c = vec2_from_vec4(point_c);

	vec3_t weights = barycentric_weights(a, b, c, point_p);
	
	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	// variables to store the interpolated values of U, V and also 1/w for the current pixel
	float interpolated_u;
	float interpolated_v;
	float interpolated_reciprocal_w;

	// perform the interpolation of all U/w and V/w values using barycentric weights and a factor of 1/w
	interpolated_u = ((u0 / point_a.w) * alpha) + ((u1 / point_b.w) * beta) + ((u2 / point_c.w) * gamma);
	interpolated_v = ((v0 / point_a.w) * alpha) + ((v1 / point_b.w) * beta) + ((v2 / point_c.w) * gamma);

	// also interpolate the values of 1/w for the current pixel
	interpolated_reciprocal_w = (a_w_reciprocal * alpha) + (b_w_reciprocal * beta) + (c_w_reciprocal * gamma);
	// now divide both of those interpolated U and V values by 1/w
	
	interpolated_u /= interpolated_reciprocal_w;
	interpolated_v /= interpolated_reciprocal_w;

	int tex_x = abs((int)(interpolated_u * texture_width));
	int tex_y = abs((int)(interpolated_v * texture_height));

	draw_pixel(x, y, texture[(texture_width * tex_y) + tex_x]);
}

void draw_textured_triangle(
	int x0, int y0, float z0, float w0, float u0, float v0,
	int x1, int y1, float z1, float w1, float u1, float v1,
	int x2, int y2, float z2, float w2, float u2, float v2,
	uint32_t* texture
) {
	// sort vertices by y-coord ascending (y0 < y1 < y2)
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
		float_swap(&z1, &z2);
		float_swap(&w1, &w2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
	}

	// create vector points after sorting
	vec4_t point_a = { x0, y0, z0, w0 };
	vec4_t point_b = { x1, y1, z1, w1 };
	vec4_t point_c = { x2, y2, z2, w2 };

	// Render the flat-bottom part of the triangle
	float inv_slope_1 = 0;
	float inv_slope_2 = 0;

	float a_w_reciprocal = 1 / point_a.w;
	float b_w_reciprocal = 1 / point_b.w;
	float c_w_reciprocal = 1 / point_c.w;

	if (y1 - y0 != 0)
		inv_slope_1 = (float)(x1 - x0) / (float)abs((y1 - y0));
	if (y2 - y0 != 0)
		inv_slope_2 = (float)(x2 - x0) / (float)abs((y2 - y0));

	if (y1 - y0 != 0) {
		for (int y = y0; y < y1; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start)
				int_swap(&x_start, &x_end);

			for (int x = x_start; x <= x_end; x++) {
				//draw_pixel(x, y, &ORANGE);
				
				draw_texel(
					x, y, texture,
					point_a, point_b, point_c,
					a_w_reciprocal, b_w_reciprocal, c_w_reciprocal,
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
		for (int y = y1; y < y2; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start)
				int_swap(&x_start, &x_end);

			for (int x = x_start; x <= x_end; x++) {
				//draw_pixel(x, y, &ORANGE);
				
				draw_texel(
					x, y, texture,
					point_a, point_b, point_c,
					a_w_reciprocal, b_w_reciprocal, c_w_reciprocal,
					u0, v0, u1, v1, u2, v2
				);
				
			}
		}
	}
}

