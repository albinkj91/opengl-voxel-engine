#ifndef CHUNK_H
#define CHUNK_H

#include "Voxel.h"
#include <vector>
#include <memory>

class Chunk
{
public:
	Chunk();

	void render();
	void init_vao();
	void init_vbo();
	void set_active();
private:
	std::vector<std::unique_ptr<Voxel>> voxels;
	unsigned int vao;
	unsigned int vbo;
};

#endif
