#define GL_GLEXT_PROTOTYPES

#include "Flower.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

Flower::Flower(glm::vec3 const& position, Program const& program, unsigned int const texture)
	:Voxel{position, program}, texture{texture}
{
}

void Flower::render() const
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glm::mat4 matrix{glm::translate(glm::mat4{1.f}, glm::vec3{position.x, position.y, -position.z - 0.5f})};
	int transform_matrix_location{glGetUniformLocation(program.get(), "transformMatrix")};
	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));

	glDrawArrays(GL_TRIANGLES, 0, 3 * 8);
}
