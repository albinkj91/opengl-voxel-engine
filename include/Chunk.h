#ifndef CHUNK_H
#define CHUNK_H

#include "Voxel.h"
#include "Voxel_Type.h"
#include <vector>
#include <memory>
#include <glm/mat4x4.hpp>

class Chunk
{
public:
	Chunk(std::vector<float> const& vertices);

	void render();
	void init_vao();
	void init_vbo();
	void create(Voxel_Type const type, glm::vec3 const& pos, Program const& program);
	unsigned int size() const;
private:
	std::vector<std::unique_ptr<Voxel>> voxels;
	std::vector<float> vertices;
	unsigned int vao;
	unsigned int vbo;

	void bind() const;
	void unbind() const;
};

#endif
