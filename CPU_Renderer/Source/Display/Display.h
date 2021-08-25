#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <stdint.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#undef main

#define FPS 30
#define FRAME_TARGET_TIME (1000 / FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;
extern std::string window_title;
extern SDL_WindowFlags window_flag;

// Function Signatures
bool setup_sdl();
bool setup_color_buffer();
void draw_pixel(int x, int y, uint32_t* color);
void draw_pixel(int x, int y, uint32_t color);
void draw_grid(int multiple, uint32_t* color);
void render_color_buffer();
void clear_color_buffer(uint32_t* color);
void draw_rectangle(int x, int y, int w, int h, uint32_t* color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t* color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t* color);
void destroy_display();

#endif