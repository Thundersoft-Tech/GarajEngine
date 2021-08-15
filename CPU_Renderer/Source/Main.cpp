#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#undef main

bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool setup_sdl() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Failed to initialize SDL.\n";
		return false;
	}

	window = SDL_CreateWindow(
		"Garaj Engine [CPU Renderer]", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_BORDERLESS
	);
	
	if (!window) {
		std::cout << "Failed to create SDL window.\n";
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		std::cout << "Failed to create SDL renderer.\n";
		return false;
	}

	return true;
}

void setup() {

}

void keyboard_key_down(SDL_KeyboardEvent key) {
	if (key.keysym.sym == SDLK_ESCAPE)
		is_running = false;
}

void keyboard_key_up(SDL_KeyboardEvent key) {

}

void mouse_button_down(SDL_MouseButtonEvent button) {
	if (button.button == SDL_BUTTON_LEFT)
		std::cout << "Pressed left mouse button at (" << button.x << ", " << button.y << ") position.\n";
}

void mouse_button_up(SDL_MouseButtonEvent button) {

}

void mouse_motion(SDL_MouseMotionEvent motion) {

}

void mouse_wheel(SDL_MouseWheelEvent wheel) {

}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
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
	SDL_SetRenderDrawColor(renderer, 240, 248, 255, 255);
	SDL_RenderClear(renderer);
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
	return 0;
}