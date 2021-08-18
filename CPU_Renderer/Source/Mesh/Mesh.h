#ifndef MESH_H
#define MESH_H
#include "../Vector/Vector.h"
#include "../Triangle/Triangle.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face
extern face_t cube_faces[N_CUBE_FACES];

typedef struct {
	std::vector<vec3_t> vertices;
	std::vector<face_t> faces;
	vec3_t position;
	vec3_t rotation;
	vec3_t scale;
} mesh_t;

extern mesh_t mesh;

// function signature
void load_cube_mesh_data();
void destroy_mesh();
bool load_obj_file_data(std::string file_name);

#endif

