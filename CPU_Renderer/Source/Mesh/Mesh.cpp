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
	FILE* file;
	fopen_s(&file, file_name.c_str(), "r");
	char line[32768];

	std::vector<tex2_t> texcoords;

	while (fgets(line, 32768, file)) {
		// Vertex information
		if (strncmp(line, "v ", 2) == 0) {
			vec3_t vertex;
			sscanf_s(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			mesh.vertices.push_back(vertex);
		}
		// Texture coordinate information
		if (strncmp(line, "vt ", 3) == 0) {
			tex2_t texcoord;
			sscanf_s(line, "vt %f %f", &texcoord.u, &texcoord.v);
			texcoords.push_back(texcoord);
		}
		// Face information
		if (strncmp(line, "f ", 2) == 0) {
			int vertex_indices[3];
			int texture_indices[3];
			int normal_indices[3];
			sscanf_s(
				line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&vertex_indices[0], &texture_indices[0], &normal_indices[0],
				&vertex_indices[1], &texture_indices[1], &normal_indices[1],
				&vertex_indices[2], &texture_indices[2], &normal_indices[2]
			);
			face_t face;
			face.a = vertex_indices[0];
			face.b = vertex_indices[1];
			face.c = vertex_indices[2];
			if (texcoords.size() > (texture_indices[0] - 1))
				face.a_uv = texcoords[texture_indices[0] - 1];
			if (texcoords.size() > (texture_indices[1] - 1))
				face.b_uv = texcoords[texture_indices[1] - 1];
			if (texcoords.size() > (texture_indices[2] - 1))
				face.c_uv = texcoords[texture_indices[2] - 1];
			face.color = WHITE;
			mesh.faces.push_back(face);
		}
	}
	mesh.scale = { 1, 1, 1 };
	
	std::cout << "Faces = " << mesh.faces.size() << std::endl;
	std::cout << "Vertices = " << mesh.vertices.size() << std::endl;

	return true;
}
