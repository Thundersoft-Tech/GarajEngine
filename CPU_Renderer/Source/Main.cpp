#include "Colors/Colors.h"
#include "Display/Display.h"
#include "Input/Input.h"
#include "Vector/Vector.h"

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

void update() {

}

void render() {
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);

	draw_rectangle(200, 200, 200, 100, &BLACK);
	render_color_buffer();
	clear_color_buffer(&ALICE_BLUE);

	SDL_RenderPresent(renderer);
}

int main() {
	is_running = setup_sdl();
	setup();
	vec3_t my_vector = { 2.0, 5.0, -7.0 };
	while (is_running)
	{
		process_input();
		update();
		render();
	}
	destroy();
	return 0;
}