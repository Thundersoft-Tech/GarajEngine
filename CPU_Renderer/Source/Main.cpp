#include "Colors/Colors.h"
#include "Display/Display.h"
#include "Input/Input.h"
#include "Vector/Vector.h"

vec3_t camera_position = { 0, 0, 5 };
vec3_t cube_rotation = { 0, 0, 0 };

float fov_factor = 640;

int previous_frame_time;

bool is_running = false;

void setup() {
	is_running = setup_color_buffer();
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

void projection() {
	cube_rotation.y += 0.01;
}

void update() {
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	previous_frame_time = SDL_GetTicks();

	projection();
}

void draw_cube() {

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