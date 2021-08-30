#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"

enum render_modes { WIREFRAME, VERTEX, FILLED, FILLED_OUTLINE, TEXTURED, TEXTURED_OUTLINE };
enum culling_modes { BACK_FACE_CULLING, FRONT_FACE_CULLING, CULLING_DISABLED };
extern render_modes render_mode;
extern culling_modes culling;

void keyboard_key_down(SDL_KeyboardEvent key);
void keyboard_key_up(SDL_KeyboardEvent key);
void mouse_button_down(SDL_MouseButtonEvent button);
void mouse_button_up(SDL_MouseButtonEvent button);
void mouse_motion(SDL_MouseMotionEvent motion);
void mouse_wheel(SDL_MouseWheelEvent wheel);

#endif
