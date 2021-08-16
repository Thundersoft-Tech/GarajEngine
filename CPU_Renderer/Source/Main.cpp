#include <iostream>
#include <stdint.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#undef main

bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// Color Buffer properties
uint32_t* color_buffer = NULL;
uint32_t ALICE_BLUE = 0xFFF0F8FF;
uint32_t BLACK = 0xFF000000;
uint32_t background_color = ALICE_BLUE;
SDL_Texture* color_buffer_texture = NULL;

// Window properties
int window_width = 800;
int window_height = 600;
std::string window_title = "CPU Renderer";
SDL_WindowFlags window_flag = SDL_WINDOW_BORDERLESS;

bool setup_sdl() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Failed to initialize SDL.\n";
		return false;
	}

	// Query current resolution of the screen
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;
	// --------------------------------------

	window = SDL_CreateWindow(
		window_title.c_str(), SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, window_width, window_height, window_flag
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

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void setup() {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t)* window_width * window_height);
	if (!color_buffer) {
		is_running = false;
		std::cout << "Failed to allocate memory for the color buffer." << std::endl;
	}
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
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

void draw_pixel(int x, int y, uint32_t * color) {
	color_buffer[(window_width * y) + x] = *color;
}

void draw_grid(int multiple = 100) {
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			if ((x % multiple == 0) or (y % multiple == 0)) {
				draw_pixel(x, y, &BLACK);
			}
		}
	}
}

void render_clear_buffer() {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)window_width * sizeof(uint32_t)
	);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer() {
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			draw_pixel(x, y, &background_color);
		}
	}
}

void render() {
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderClear(renderer);

	draw_grid();
	render_clear_buffer();
	clear_color_buffer();

	SDL_RenderPresent(renderer);
}

void destroy() {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
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