#include "Colors/Colors.h"
#include "Display/Display.h"
#include "Input/Input.h"
#include "Vector/Vector.h"
#include "Mesh/Mesh.h"
#include <vector>
#include <math.h>

std::vector<triangle_t> triangles_to_render;

vec3_t camera_position = { 0, 0, 0 };

float fov_factor = 640;

int previous_frame_time;

bool is_running = false;

void setup() {
	is_running = setup_color_buffer();
	// load_cube_mesh_data();
	is_running = load_obj_file("./Assets/Models/Cube/Cube.obj");
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

void projection(int count = 0) {
	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;

	// loop all mesh faces
	for (int i = 0; i < mesh.faces.size(); i++) {
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		triangle_t projected_triangle;
		vec3_t transformed_vertices[3];

		// loop all 3 vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];
			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			// translate vertex away from camera
			transformed_vertex.z += 5.0;

			// save vertex
			transformed_vertices[j] = transformed_vertex;
		}

		if (culling != CULLING_DISABLED){
			// Back-Face Culling
			vec3_t vector_a = transformed_vertices[0];
			vec3_t vector_b = transformed_vertices[1];
			vec3_t vector_c = transformed_vertices[2];

			vec3_t vector_ab = vec3_subtract(vector_b, vector_a);
			vec3_t vector_ac = vec3_subtract(vector_c, vector_a);

			vec3_normalize(&vector_ab);
			vec3_normalize(&vector_ac);

			vec3_t normal_vector = vec3_cross(vector_ab, vector_ac);

			vec3_normalize(&normal_vector);

			vec3_t camera_ray_vector = vec3_subtract(camera_position, vector_a);

			float dot_product = vec3_dot(normal_vector, camera_ray_vector);

			if (culling == BACK_FACE_CULLING) {
				if (dot_product < 0)
					continue;
			}
			else
			{
				if (dot_product > 0)
					continue;
			}
		}
		// loop all vertices and perform projection
		for (int j = 0; j < 3; j++){
			// project the current vertex
			vec2_t projected_point = project(transformed_vertices[j]);

			// scale and translate projected point
			projected_point.x += (window_width / 2.0);
			projected_point.y += (window_height / 2.0);
			
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

	triangles_to_render.clear();

	projection();
}

void draw_wireframe() {
	for (int i = 0; i < triangles_to_render.size(); i++) {
		triangle_t triangle = triangles_to_render[i];
		draw_triangle(
			triangle.points[0].x, triangle.points[0].y,
			triangle.points[1].x, triangle.points[1].y,
			triangle.points[2].x, triangle.points[2].y,
			&WHITE
		);
		if (render_mode == VERTEX) {
			for (int j = 0; j < 3; j++) {
				draw_rectangle(
					triangle.points[j].x, triangle.points[j].y, 10, 10, &WHITE
				);
			}
		}
	}
}

void draw_triangles() {
	for (int i = 0; i < triangles_to_render.size(); i++) {
		triangle_t triangle = triangles_to_render[i];
		draw_filled_triangle(
			triangle.points[0].x, triangle.points[0].y,
			triangle.points[1].x, triangle.points[1].y,
			triangle.points[2].x, triangle.points[2].y,
			&WHITE
		);
		if (render_mode == FILLED_OUTLINE) {
			draw_triangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				&BLACK
			);
		}
	}
}

void draw_mesh() {
	if (render_mode == FILLED_OUTLINE || render_mode == FILLED)
		draw_triangles();
	if (render_mode == WIREFRAME || render_mode == VERTEX)
		draw_wireframe();
}

void render() {
	draw_mesh();
	render_color_buffer();
	clear_color_buffer(&DARK_GRAY);

	SDL_RenderPresent(renderer);
}

void destroy() {
	destroy_display();
	destroy_mesh();
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