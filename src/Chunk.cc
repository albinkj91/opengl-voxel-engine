#define GL_GLEXT_PROTOTYPES

#include "Chunk.h"
#include <algorithm>
#include <iostream>

using namespace std;

Chunk::Chunk(vector<float> const& vertices)
	:voxels{}, vertices{vertices}, vao{}, vbo{}
{ }

void Chunk::render()
{
	bind();
	for_each(voxels.begin(), voxels.end(),
		[this](unique_ptr<Voxel> const& voxel)
		{
			voxel->render();
		});
	unbind();
}

void Chunk::init_vao()
{
	glGenVertexArrays(1, &vao);
	bind();
}

void Chunk::init_vbo()
{
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            vertices.data(),
            GL_STATIC_DRAW);

	auto texture_offset{sizeof(float) * static_cast<int>(vertices.size() * (4.f/9.f))};
	auto normal_offset{sizeof(float) * static_cast<int>(vertices.size() * (2.f/3.f))};
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)texture_offset);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)normal_offset);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Chunk::add(unique_ptr<Voxel> voxel)
{
	voxels.push_back(move(voxel));
}

unsigned int Chunk::size() const
{
	return voxels.size();
}

void Chunk::bind() const
{
	glBindVertexArray(vao);
}

void Chunk::unbind() const
{
	glBindVertexArray(0);
}
