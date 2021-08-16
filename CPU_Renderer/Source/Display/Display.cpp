#include "Display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
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

bool setup_color_buffer() {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
	if (!color_buffer) {
		std::cout << "Failed to allocate memory for the color buffer." << std::endl;
		return false;
	}
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
	if (!color_buffer_texture) {
		std::cout << "Failed to allocate memory for the color buffer texture." << std::endl;
		return false;
	}
	return true;
}

void draw_pixel(int x, int y, uint32_t* color) {
	color_buffer[(window_width * y) + x] = *color;
}

void draw_grid(int multiple, uint32_t* color) {
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			if ((x % multiple == 0) or (y % multiple == 0)) {
				draw_pixel(x, y, color);
			}
		}
	}
}

void render_color_buffer() {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)window_width * sizeof(uint32_t)
	);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t* color) {
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			draw_pixel(x, y, color);
		}
	}
}

void draw_rectangle(int x, int y, int w, int h, uint32_t* color) {
	for (int i = y; i < y + h; i++) {
		for (int j = x; j < x + w; j++) {
			draw_pixel(j, i, color);
		}
	}
}