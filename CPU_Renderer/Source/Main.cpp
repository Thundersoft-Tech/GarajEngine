#include "Colors/Colors.h"
#include "Display/Display.h"
#include "Input/Input.h"
#include "Vector/Vector.h"
#include "Mesh/Mesh.h"
#include <vector>

std::vector<triangle_t> triangles_to_render;

vec3_t camera_position = { 0, 0, -5 };

float fov_factor = 640;

int previous_frame_time;

bool is_running = false;

void setup() {
	is_running = setup_color_buffer();
	load_cube_mesh_data();
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
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

vec2_t project(vec3_t point) {
	vec2_t projected_point = { (fov_factor * point.x) / point.z, (fov_factor * point.y) / point.z };
	return projected_point;
}

void projection() {
	mesh.rotation.y += 0.01;

	triangles_to_render.clear();

	// loop all mesh faces
	for (int i = 0; i < N_CUBE_FACES; i++) {
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		triangle_t projected_triangle;

		// loop all 3 vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);

			// translate vertex away from camera
			transformed_vertex.z -= camera_position.z;

			// project the current vertex
			vec2_t projected_point = project(transformed_vertex);

			// scale and translate projected point
			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);
			
			projected_triangle.points[j] = projected_point;
		}

		// save the projected triangle in the array of triangles to render
		triangles_to_render.push_back(projected_triangle);

	}

}

void update() {
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	previous_frame_time = SDL_GetTicks();

	projection();
}

void draw_cube() {
	// loop all the projected triangles and render them
	for (int i = 0; i < triangles_to_render.size(); i++) {
		triangle_t triangle = triangles_to_render[i];
		draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, &BLACK);
		draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, &BLACK);
		draw_line(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, &BLACK);
		for (int j = 0; j < 3; j++) {
			draw_rectangle(
				triangle.points[j].x, triangle.points[j].y, 10, 10, &BLACK
			);
		}
	}
}

void render() {
	draw_cube();
	render_color_buffer();
	clear_color_buffer(&ALICE_BLUE);

	SDL_RenderPresent(renderer);
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