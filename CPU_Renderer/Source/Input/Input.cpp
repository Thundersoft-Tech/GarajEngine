#include <iostream>
#include "Input.h"
#include "../Display/Display.h"
#include "../Colors/Colors.h"

void keyboard_key_down(SDL_KeyboardEvent key) {

}

void keyboard_key_up(SDL_KeyboardEvent key) {

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