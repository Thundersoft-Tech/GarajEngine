#include <iostream>
#include "Input.h"
#include "../Display/Display.h"
#include "../Colors/Colors.h"

render_modes render_mode = TEXTURED;
culling_modes culling = BACK_FACE_CULLING;

void keyboard_key_down(SDL_KeyboardEvent key) {

}

void keyboard_key_up(SDL_KeyboardEvent key) {
	if (key.keysym.sym == SDLK_1)
		render_mode = VERTEX;
	if (key.keysym.sym == SDLK_2)
		render_mode = WIREFRAME;
	if (key.keysym.sym == SDLK_3)
		render_mode = FILLED;
	if (key.keysym.sym == SDLK_4)
		render_mode = FILLED_OUTLINE;
	if (key.keysym.sym == SDLK_5)
		render_mode = TEXTURED;
	if (key.keysym.sym == SDLK_6)
		render_mode = TEXTURED_OUTLINE;
	if (key.keysym.sym == SDLK_b)
		culling = BACK_FACE_CULLING;
	if (key.keysym.sym == SDLK_f)
		culling = FRONT_FACE_CULLING;
	if (key.keysym.sym == SDLK_d)
		culling = CULLING_DISABLED;
}

void mouse_button_down(SDL_MouseButtonEvent button) {
	if (button.button == SDL_BUTTON_LEFT)
		draw_rectangle(button.x, button.y, 20, 20, &RED);
		std::cout << "Pressed left mouse button at (" << button.x << ", " << button.y << ") position.\n";
}

void mouse_button_up(SDL_MouseButtonEvent button) {

}

void mouse_motion(SDL_MouseMotionEvent motion) {

}

void mouse_wheel(SDL_MouseWheelEvent wheel) {

}