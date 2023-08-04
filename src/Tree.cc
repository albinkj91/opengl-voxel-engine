#define GL_GLEXT_PROTOTYPES

#include "Tree.h"
#include "Voxel_Type.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Tree::Tree(glm::vec3 const& position,
		Program const& program,
		int const height,
		unsigned int const tx_leaves,
		unsigned int const tx_bark,
		unsigned int const tx_rings)
	:Voxel{position, program},
	height{height},
	texture_leaves{tx_leaves},
	texture_bark{tx_bark},
	texture_rings{tx_rings}
{ }

void Tree::render() const
{
	glBindTexture(GL_TEXTURE_2D, texture_bark);
	draw_trunk();

	glBindTexture(GL_TEXTURE_2D, texture_leaves);
	draw_leaves(glm::vec3{0.f, height, 0.f});
	draw_leaves(glm::vec3{1.f, height - 1, -1.f});
	draw_leaves(glm::vec3{1.f, height - 1, 0.f});
	draw_leaves(glm::vec3{1.f, height - 1, 1.f});
	draw_leaves(glm::vec3{0.f, height - 1, -1.f});
	draw_leaves(glm::vec3{0.f, height - 1, 1.f});
	draw_leaves(glm::vec3{-1.f, height - 1, -1.f});
	draw_leaves(glm::vec3{-1.f, height - 1, 0.f});
	draw_leaves(glm::vec3{-1.f, height - 1, 1.f});
}

void Tree::draw_trunk() const
{
	for(int i{}; i < height; ++i)
	{
		glm::mat4 matrix{
			glm::translate(glm::mat4{1.f}, glm::vec3{position.x, position.y + i, -position.z - 0.5f})};

		int transform_matrix_location{glGetUniformLocation(program.get(), "transformMatrix")};
		glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
	}
}

void Tree::draw_leaves(glm::vec3 const& offset) const
{
	glm::mat4 matrix{glm::translate(glm::mat4{1.f},
			glm::vec3{position.x + offset.x,
			position.y + offset.y,
			-position.z + offset.z - 0.5f})};

	int transform_matrix_location{glGetUniformLocation(program.get(), "transformMatrix")};
	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));

	glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
}
