#include "Mesh.h"

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{ -1, -1, -1 }, // 1
	{ -1,  1, -1 }, // 2
	{  1,  1, -1 }, // 3
	{  1, -1, -1 }, // 4
	{  1,  1,  1 }, // 5
	{  1, -1,  1 }, // 6
	{ -1,  1,  1 }, // 7
	{ -1, -1,  1 }, // 8
};

face_t cube_faces[N_CUBE_FACES] = {
	// front
	{ 1, 2, 3, { 0, 1 }, { 0, 0 }, { 1, 0 }, WHITE },
	{ 1, 3, 4, { 0, 1 }, { 1, 0 }, { 1, 1 }, WHITE },
	// right
	{ 4, 3, 5, { 0, 1 }, { 0, 0 }, { 1, 0 }, WHITE },
	{ 4, 5, 6, { 0, 1 }, { 1, 0 }, { 1, 1 }, WHITE },
	// back
	{ 6, 5, 7, { 0, 1 }, { 0, 0 }, { 1, 0 }, WHITE },
	{ 6, 7, 8, { 0, 1 }, { 1, 0 }, { 1, 1 }, WHITE },
	// left
	{ 8, 7, 2, { 0, 1 }, { 0, 0 }, { 1, 0 }, WHITE },
	{ 8, 2, 1, { 0, 1 }, { 1, 0 }, { 1, 1 }, WHITE },
	// top
	{ 2, 7, 5, { 0, 1 }, { 0, 0 }, { 1, 0 }, WHITE },
	{ 2, 5, 3, { 0, 1 }, { 1, 0 }, { 1, 1 }, WHITE },
	// bottom
	{ 6, 8, 1, { 0, 1 }, { 0, 0 }, { 1, 0 }, WHITE },
	{ 6, 1, 4, { 0, 1 }, { 1, 0 }, { 1, 1 }, WHITE },
};

mesh_t mesh;

void load_cube_mesh_data() {
	for (int i = 0; i < N_CUBE_VERTICES; i++) {
		mesh.vertices.push_back(cube_vertices[i]);
	}
	for (int i = 0; i < N_CUBE_FACES; i++) {
		mesh.faces.push_back(cube_faces[i]);
	}
	mesh.scale = { 1, 1, 1 };
}

void destroy_mesh() {
	mesh.faces.clear();
	mesh.vertices.clear();
}

bool load_obj_file(std::string file_name) {
	std::ifstream obj_file;
	//obj_file.open(file_name, std::ios::in);
	obj_file.open(file_name);
	if (!obj_file) {
		std::cout << "loading " << file_name << " failed.\n";
		return false;
	}
	else {
		face_t face;
		vec3_t vertex;
		while (obj_file) {
			std::string line;
			std::getline(obj_file, line);
			if (line[0] == 'f' && line[1] == ' ') {
				std::stringstream line_stream(line);
				std::string portion;
				face_t face;
				// f 1/1/1 5/2/1 7/3/1 3/4/1
				std::vector<int> face_values;
				int i = 0;
				while (std::getline(line_stream, portion, ' ')) {
					if (i > 0) {
						std::stringstream portion_stream(portion);
						std::string value;
						while (std::getline(portion_stream, value, '/')) {
							face_values.push_back(std::stoi(value));
							break;
						}
					}
					i += 1;
				}
				face.a = face_values[0];
				face.b = face_values[1];
				face.c = face_values[2];
				face.color = WHITE;
				mesh.faces.push_back(face);
			}
			if (line[0] == 'v' && line[1] == ' ') {
				std::stringstream line_stream(line);
				std::string portion;
				int i = 0;
				while (std::getline(line_stream, portion, ' ')) {
					if (i == 1)
						vertex.x = std::stof(portion);
					if (i == 2)
						vertex.y = std::stof(portion);
					if (i == 3)
						vertex.z = std::stof(portion);
					i += 1;
				}
				mesh.vertices.push_back(vertex);
			}
		}
	}
	obj_file.close();
	mesh.scale = { 1, 1, 1 };
	return true;
}