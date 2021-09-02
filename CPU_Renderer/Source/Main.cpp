#include <vector>
#include <math.h>
#include "Colors/Colors.h"
#include "Display/Display.h"
#include "Input/Input.h"
#include "Vector/Vector.h"
#include "Mesh/Mesh.h"
#include "upng/upng.h"
#include "Matrix/Matrix.h"
#include "Light/Light.h"
#include "Camera/Camera.h"

#define MAX_TRIANGLES 10000
triangle_t triangles_to_render[MAX_TRIANGLES];
int number_of_triangles = 0;

//float fov_factor = 640;
mat4_t proj_matrix;
mat4_t world_matrix;
mat4_t view_matrix;
int previous_frame_time;
float delta_time;
bool is_running = false;

void setup() {
	is_running = setup_color_buffer();
	float fov = M_PI / 3.0;
	float aspect = (float)window_height / (float)window_width;
	float z_near = 0.1;
	float z_far = 100.0;
	proj_matrix = mat4_make_perspective(fov, aspect, z_near, z_far);

	//mesh_texture = (uint32_t*)REDBRICK_TEXTURE;

	//load_cube_mesh_data();
	is_running = load_obj_file("./Assets/Models/Others/f22.obj");

	std::string file_name = "./Assets/Models/Others/f22.png";

	load_png_texture_data(file_name.c_str());
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
		if (event.key.keysym.sym == SDLK_UP)
			camera.position.y += 3.0 * delta_time;
		if (event.key.keysym.sym == SDLK_DOWN)
			camera.position.y -= 3.0 * delta_time;
		if (event.key.keysym.sym == SDLK_a)
			camera.yaw_angle += 1.0 * delta_time;
		if (event.key.keysym.sym == SDLK_d)
			camera.yaw_angle -= 1.0 * delta_time;
		if (event.key.keysym.sym == SDLK_w) {
			camera.forward_velocity = vec3_multiply(camera.direction, 5.0 * delta_time);
			camera.position = vec3_add(camera.position, camera.forward_velocity);
		}
		if (event.key.keysym.sym == SDLK_s) {
			camera.forward_velocity = vec3_multiply(camera.direction, 5.0 * delta_time);
			camera.position = vec3_subtract(camera.position, camera.forward_velocity);
		}
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
	//mesh.rotation.x += 0.6 * delta_time;
	//mesh.rotation.y += 0.6 * delta_time;
	//mesh.rotation.z += 0.6 * delta_time;

	//mesh.scale.x += 0.002 * delta_time;
	//mesh.scale.y += 0.001 * delta_time;

	//mesh.translation.y = -1.5;
	mesh.translation.z = 4.5; // move away from the camera

	// change camera_position per frame
	camera.position.x += 0.0 * delta_time;
	camera.position.y += 0.0 * delta_time;

	mat4_t camera_yaw_rotation = mat4_make_rotation_y(camera.yaw_angle);
	vec3_t target = { 0, 0, 1 };
	camera.direction = vec3_from_vec4(mat4_mul_vec4(camera_yaw_rotation, vec4_from_vec3(target)));
	
	target = vec3_add(camera.position, camera.direction);
	vec3_t up_direction = { 0, 1,0 };

	// create view matrix
	view_matrix = mat4_look_at(camera.position, target, up_direction);

	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);


	world_matrix = mat4_identity();
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

			// multiply the view matrix
			transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);

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
		
		vec3_t origin = { 0, 0, 0 };
		vec3_t camera_ray_vector = vec3_subtract(origin, vector_a);

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
		if (number_of_triangles < MAX_TRIANGLES) {
			// save the projected triangle in the array of triangles to render
			triangles_to_render[number_of_triangles] = projected_triangle;
			number_of_triangles += 1;
		}
	}
}

void update() {
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0;

	previous_frame_time = SDL_GetTicks();

	number_of_triangles = 0;

	projection();
}

void draw_wireframe() {
	for (int i = 0; i < number_of_triangles; i++) {
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

void draw_textured_triangles() {
	for (int i = 0; i < number_of_triangles; i++) {
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

void draw_filled_triangles() {
	for (int i = 0; i < number_of_triangles; i++) {
		triangle_t triangle = triangles_to_render[i];
		draw_filled_triangle(
			triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w,
			triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w,
			triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
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

void draw_mesh() {
	if (render_mode == FILLED_OUTLINE || render_mode == FILLED)
		draw_filled_triangles();
	if (render_mode == TEXTURED_OUTLINE || render_mode == TEXTURED)
		if (mesh_texture == NULL) {
			draw_filled_triangles();
		}
		else
		{
			draw_textured_triangles();
		}
	if (render_mode == WIREFRAME || render_mode == VERTEX)
		draw_wireframe();
}

void render() {
	draw_mesh();
	render_color_buffer();
	clear_color_buffer(&DARK_GRAY);
	clear_z_buffer();
	SDL_RenderPresent(renderer);
}

void destroy() {
	destroy_display();
	destroy_mesh();
	if (png_texture != NULL)
		upng_free(png_texture);
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