#include "Colors/Colors.h"
#include "Display/Display.h"
#include "Input/Input.h"
#include "Vector/Vector.h"
#include "Mesh/Mesh.h"
#include <vector>
#include <math.h>
#include "Matrix/Matrix.h"
#include "Light/Light.h"

std::vector<triangle_t> triangles_to_render;

vec3_t camera_position = { 0, 0, 0 };

//float fov_factor = 640;
mat4_t proj_matrix;

int previous_frame_time;

bool is_running = false;

void setup() {
	is_running = setup_color_buffer();
	float fov = M_PI / 3.0;
	float aspect = (float)window_height / (float)window_width;
	float z_near = 0.1;
	float z_far = 100.0;
	proj_matrix = mat4_make_perspective(fov, aspect, z_near, z_far);

	mesh_texture = (uint32_t*)REDBRICK_TEXTURE;

	load_cube_mesh_data();
	//is_running = load_obj_file("./Assets/Models/Plane/F22.obj");
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

void projection(int count = 0) {
	//mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	//mesh.rotation.z += 0.01;

	//mesh.scale.x += 0.002;
	//mesh.scale.y += 0.001;

	//mesh.translation.x += 0.01;
	mesh.translation.z = 4.0; // move away from the camera

	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);


	mat4_t world_matrix = mat4_identity();
	world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
	world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
	world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
	world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
	world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

	// loop all mesh faces
	for (int i = 0; i < mesh.faces.size(); i++) {
		face_t mesh_face = mesh.faces[i];
		uint32_t face_color = mesh.faces[i].color;

		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		triangle_t projected_triangle;
		vec4_t transformed_vertices[3];

		// loop all 3 vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++) {
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

			transformed_vertices[j] = transformed_vertex;
		}

		// Back-Face Culling
		vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
		vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
		vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

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
		else if(culling == FRONT_FACE_CULLING)
		{
			if (dot_product > 0)
				continue;
		}

		// loop all vertices and perform projection
		for (int j = 0; j < 3; j++){
			// project the current vertex
			vec4_t projected_point = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);// = project(vec3_from_vec4(transformed_vertices[j]));

			// scaling projected point

			projected_point.x *= (window_width / 2.0);
			projected_point.y *= (window_height / 2.0);

			// invert y values
			projected_point.y *= -1;

			// translating projected point
			projected_point.x += (window_width / 2.0);
			projected_point.y += (window_height / 2.0);
			
			projected_triangle.points[j] = projected_point;
		}

		projected_triangle.texcoords[0] = mesh_face.a_uv;
		projected_triangle.texcoords[1] = mesh_face.b_uv;
		projected_triangle.texcoords[2] = mesh_face.c_uv;
		
		vec3_normalize(&light.direction);
		float percentage_factor = -vec3_dot(normal_vector, light.direction);
		uint32_t shaded_color = face_color;

		shaded_color = apply_light_intensity(shaded_color, percentage_factor);

		projected_triangle.color = shaded_color;
		projected_triangle.average_depth = (float)((transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3);
		
		// save the projected triangle in the array of triangles to render
		triangles_to_render.push_back(projected_triangle);
	}

	// Sort triangles based on depth
	bool swapped = false;
	do
	{
		swapped = false;
		for (int i = 1; i < triangles_to_render.size(); i++) {
			if (triangles_to_render[i - 1].average_depth < triangles_to_render[i].average_depth) {
				triangle_t tmp = triangles_to_render[i - 1];
				triangles_to_render[i - 1] = triangles_to_render[i];
				triangles_to_render[i] = tmp;
				swapped = true;
			}
		}
	} while (swapped);
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
					triangle.points[j].x, triangle.points[j].y, 10, 10, &ORANGE
				);
			}
		}
	}
}

void draw_filled_triangles() {
	for (int i = 0; i < triangles_to_render.size(); i++) {
		triangle_t triangle = triangles_to_render[i];
		draw_filled_triangle(
			triangle.points[0].x, triangle.points[0].y,
			triangle.points[1].x, triangle.points[1].y,
			triangle.points[2].x, triangle.points[2].y,
			triangle.color
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

void draw_textured_triangles() {
	for (int i = 0; i < triangles_to_render.size(); i++) {
		triangle_t triangle = triangles_to_render[i];
		draw_textured_triangle(
			triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v,
			triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v,
			triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v,
			mesh_texture
		);
		if (render_mode == TEXTURED_OUTLINE) {
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
		draw_filled_triangles();
	if (render_mode == TEXTURED_OUTLINE || render_mode == TEXTURED)
		draw_textured_triangles();
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