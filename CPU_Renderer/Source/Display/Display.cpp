#include "Display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
float* z_buffer = NULL;
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
	//window_width = display_mode.w;
	//window_height = display_mode.h;
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

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

bool setup_color_buffer() {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
	z_buffer = (float*)malloc(sizeof(float) * window_width * window_height);
	if (!z_buffer) {
		std::cout << "Failed to allocate memory for the z buffer." << std::endl;
		return false;
	}
	if (!color_buffer) {
		std::cout << "Failed to allocate memory for the color buffer." << std::endl;
		return false;
	}
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
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
	if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
		color_buffer[(window_width * y) + x] = *color;
	}
}


void draw_pixel(int x, int y, uint32_t color) {
	if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
		color_buffer[(window_width * y) + x] = color;
	}
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

void clear_color_buffer(uint32_t color) {
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			draw_pixel(x, y, color);
		}
	}
}

void clear_z_buffer() {
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			z_buffer[(window_width * y) + x] = 1.0;
		}
	}
}

void draw_rectangle(int x, int y, int w, int h, uint32_t* color) {
	for (int i = y; i < y + h; i++) {
		for (int j = x; j < x + w; j++) {
			draw_pixel(j - (w/2), i - (h/2), color);
		}
	}
}

void draw_rectangle(int x, int y, int w, int h, uint32_t color) {
	for (int i = y; i < y + h; i++) {
		for (int j = x; j < x + w; j++) {
			draw_pixel(j - (w/2), i - (h/2), color);
		}
	}
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t* color) {
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x2, y2, x0, y0, color);
}


void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x2, y2, x0, y0, color);
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t* color) {
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)side_length;
	float y_inc = delta_y / (float)side_length;

	float x = x0;
	float y = y0;

	for (int i = 0; i < side_length; i++) {
		draw_pixel(x, y, color);
		x += x_inc;
		y += y_inc;
	}
}


void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)side_length;
	float y_inc = delta_y / (float)side_length;

	float x = x0;
	float y = y0;

	for (int i = 0; i < side_length; i++) {
		draw_pixel(x, y, color);
		x += x_inc;
		y += y_inc;
	}
}

void destroy_display() {
	free(color_buffer);
	free(z_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
