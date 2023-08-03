#define GL_GLEXT_PROTOTYPES

#include "Chunk.h"
#include "Grass.h"
#include "Tree.h"
#include <SFML/OpenGL.hpp>
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

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(4 * 4 * 3 * 12));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Chunk::create(Voxel_Type const type, glm::vec3 const& pos, Program const& program)
{
	switch(type)
	{
		case Voxel_Type::grass:
			voxels.push_back(make_unique<Grass>(Grass{
				pos,
				program,
				Texture::load("assets/grass.png"),
				Texture::load("assets/dirt-grass.png"),
				Texture::load("assets/dirt.png")}));
			break;
		case Voxel_Type::dirt:
			break;
		case Voxel_Type::flower:
			break;
		case Voxel_Type::tree:
			voxels.push_back(make_unique<Tree>(Tree{
				pos,
				program,
				5,
				Texture::load("assets/grass.png"),
				Texture::load("assets/dirt.png"),
				Texture::load("assets/dirt.png")}));
			break;
		default:
			cout << "No matching voxel type." << endl;
			break;
	}
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
