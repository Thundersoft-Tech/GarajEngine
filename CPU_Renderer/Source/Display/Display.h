#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <stdint.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#undef main

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
void draw_grid(int multiple, uint32_t* color);
void render_color_buffer();
void clear_color_buffer(uint32_t* color);
void draw_rectangle(int x, int y, int w, int h, uint32_t* color);
void destroy();

#endif