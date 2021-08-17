#include "Colors/Colors.h"
#include "Display/Display.h"
#include "Input/Input.h"
#include "Vector/Vector.h"

const int N_POINTS = 9 * 9 * 9;
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];
vec3_t camera_position = { 0, 0, -5 };

float fov_factor = 640;

bool is_running = false;

void setup_points() {
	// load array of vectors
	// from -1 to 1
	int point_count = 0;
	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3_t new_point = { x, y, z };
				cube_points[point_count] = new_point;
				point_count += 1;
			}
		}
	}
}

void setup() {
	is_running = setup_color_buffer();
	setup_points();
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		keyboard_key_down(event.key);
		break;
	case SDL_KEYUP:
		keyboard_key_up(event.key);
		break;
	case SDL_MOUSEBUTTONDOWN:
		mouse_button_down(event.button);
		break;
	case SDL_MOUSEBUTTONUP:
		mouse_button_up(event.button);
		break;
	case SDL_MOUSEMOTION:
		mouse_motion(event.motion);
		break;
	case SDL_MOUSEWHEEL:
		mouse_wheel(event.wheel);
		break;
	}
}

vec2_t project(vec3_t point) {
	vec2_t projected_point = { (fov_factor * point.x) / point.z, (fov_factor * point.y) / point.z };
	return projected_point;
}

void orthographic_projection() {
	for (int i = 0; i < N_POINTS; i++) {
		vec3_t point = cube_points[i];
		point.z -= camera_position.z;
		vec2_t projected_point = project(point);
		projected_points[i] = projected_point;
	}
}

void update() {
	orthographic_projection();
}

void draw_cube() {
	for (int i = 0; i < N_POINTS; i++) {
		vec2_t projected_point = projected_points[i];
		draw_rectangle(
			projected_point.x + (window_width / 2), projected_point.y + (window_height / 2), 10, 10, &BLACK
		);
	}
}

void render() {
	draw_cube();
	render_color_buffer();
	clear_color_buffer(&ALICE_BLUE);

	SDL_RenderPresent(renderer);
}

int main() {
	is_running = setup_sdl();
	setup();
	while (is_running)
	{
		process_input();
		update();
		render();
	}
	destroy();
	return 0;
}