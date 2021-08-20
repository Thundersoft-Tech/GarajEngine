#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"

extern bool wireframe;

void keyboard_key_down(SDL_KeyboardEvent key);
void keyboard_key_up(SDL_KeyboardEvent key);
void mouse_button_down(SDL_MouseButtonEvent button);
void mouse_button_up(SDL_MouseButtonEvent button);
void mouse_motion(SDL_MouseMotionEvent motion);
void mouse_wheel(SDL_MouseWheelEvent wheel);

#endif
