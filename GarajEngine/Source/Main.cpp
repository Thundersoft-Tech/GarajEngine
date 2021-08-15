#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#undef main

void startup_message() {
	std::string platform = "32 bit";
	std::string configuration = "release";
#ifdef _WIN64
	platform = "64 bit";
#endif
#ifdef _DEBUG
	configuration = "debug";
#endif
	std::cout << "Garaj Engine " << configuration << " version running on a " << platform << " platform" << std::endl;
}

int main() {
	startup_message();
	return 0;
}