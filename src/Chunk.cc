#define GL_GLEXT_PROTOTYPES

#include "Chunk.h"
#include "Grass.h"
#include <SFML/OpenGL.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

Chunk::Chunk()
	:voxels{}, vertices{}, vao{}, vbo{}
{ }

void Chunk::render(Program const& program)
{
	bind();
	for_each(voxels.begin(), voxels.end(),
		[this, &program](unique_ptr<Voxel> const& voxel)
		{
			glm::vec3 pos{voxel->get_position()};
			glm::mat4 matrix{translate_matrix(glm::vec3{pos.x, pos.y, -pos.z - 0.5f})};
			int transform_matrix_location{glGetUniformLocation(program.get(), "transformMatrix")};
			glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
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

void Chunk::load_vertices()
{
	vector<float> voxel_vertices{
		-0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, -0.5f, 0.5f, 1.f,

		-0.5f, 0.5f, 0.5f, 1.f,
		0.5f, -0.5f, 0.5f, 1.f,
		-0.5f, -0.5f, 0.5f, 1.f,

		-0.5f, 0.5f, -0.5f, 1.f,
		-0.5f, 0.5f, 0.5f, 1.f,
		-0.5f, -0.5f, 0.5f, 1.f,

		-0.5f, 0.5f, -0.5f, 1.f,
		-0.5f, -0.5f, 0.5f, 1.f,
		-0.5f, -0.5f, -0.5f, 1.f,

		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, 0.5f, -0.5f, 1.f,
		0.5f, -0.5f, -0.5f, 1.f,

		0.5f, 0.5f, 0.5f, 1.f,
		0.5f, -0.5f, -0.5f, 1.f,
		0.5f, -0.5f, 0.5f, 1.f,

		0.5f, 0.5f, -0.5f, 1.f,
		-0.5f, 0.5f, -0.5f, 1.f,
		-0.5f, -0.5f, -0.5f, 1.f,

		0.5f, 0.5f, -0.5f, 1.f,
		-0.5f, -0.5f, -0.5f, 1.f,
		0.5f, -0.5f, -0.5f, 1.f,

		-0.5f, -0.5f, 0.5f, 1.f,
		0.5f, -0.5f, 0.5f, 1.f,
		0.5f, -0.5f, -0.5f, 1.f,

		-0.5f, -0.5f, 0.5f, 1.f,
		0.5f, -0.5f, -0.5f, 1.f,
		-0.5f, -0.5f, -0.5f, 1.f,

		-0.5f, 0.5f, -0.5f, 1.f,
		0.5f, 0.5f, -0.5f, 1.f,
		0.5f, 0.5f, 0.5f, 1.f,

		-0.5f, 0.5f, -0.5f, 1.f,
		0.5f, 0.5f, 0.5f, 1.f,
		-0.5f, 0.5f, 0.5f, 1.f,

		0.f,  1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f,  1.f,
		1.f, 0.f,
		0.f,  0.f,

		0.f,  1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f,  1.f,
		1.f, 0.f,
		0.f,  0.f,

		0.f,  1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f,  1.f,
		1.f, 0.f,
		0.f,  0.f,

		0.f,  1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f,  1.f,
		1.f, 0.f,
		0.f,  0.f,

		0.f,  1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f,  1.f,
		1.f, 0.f,
		0.f,  0.f,

		0.f,  1.f,
		1.f, 1.f,
		1.f, 0.f,

		0.f,  1.f,
		1.f, 0.f,
		0.f,  0.f
	};

	for_each(voxels.begin(), voxels.end(),
		[this, &voxel_vertices](unique_ptr<Voxel> const&)
		{
			for_each(voxel_vertices.begin(), voxel_vertices.end(),
				[this](float const vertex)
				{
					vertices.push_back(vertex);
				});
		});
}

void Chunk::create_voxel(Voxel_Type const type, glm::vec3 const& pos)
{
	switch(type)
	{
		case Voxel_Type::grass:
			voxels.push_back(make_unique<Grass>(Grass{
				pos,
				Texture::load("assets/grass.png"),
				Texture::load("assets/dirt-grass.png"),
				Texture::load("assets/dirt.png")}));
			break;
		case Voxel_Type::dirt:
			break;
		case Voxel_Type::flower:
			break;
		case Voxel_Type::wood:
			break;
		case Voxel_Type::leaves:
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

glm::mat4 Chunk::translate_matrix(glm::vec3 const& translate)
{
    glm::mat4 matrix{1.f};

    matrix[3].x = translate.x;
    matrix[3].y = translate.y;
    matrix[3].z = translate.z;
    return matrix;
}
