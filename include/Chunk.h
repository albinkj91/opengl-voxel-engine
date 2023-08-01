#ifndef CHUNK_H
#define CHUNK_H

#include "Voxel.h"
#include "Voxel_Type.h"
#include "Program.h"
#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>

class Chunk
{
public:
	Chunk();

	void render(Program const& program);
	void init_vao();
	void init_vbo();
	void load_vertices();
	void create_voxel(Voxel_Type const type, glm::vec3 const& pos);
	unsigned int size() const;
private:
	std::vector<std::unique_ptr<Voxel>> voxels;
	std::vector<float> vertices;
	unsigned int vao;
	unsigned int vbo;

	void bind() const;
	void unbind() const;
	glm::mat4 translate_matrix(glm::vec3 const& translate);
};

#endif
