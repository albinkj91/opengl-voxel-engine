#define GL_GLEXT_PROTOTYPES

#include "Grass.h"
#include <SFML/OpenGL.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

Grass::Grass(glm::vec3 const& position,
		Program const& program,
		unsigned const int tx_top,
		unsigned const int tx_sides,
		unsigned const int tx_bottom)
	:Voxel{position, program}, texture_top{tx_top}, texture_sides{tx_sides}, texture_bottom{tx_bottom}
{ }

void Grass::render() const
{
	glm::mat4 matrix{glm::translate(glm::mat4{1.f}, glm::vec3{position.x, position.y, -position.z - 0.5f})};
	int transform_matrix_location{glGetUniformLocation(program.get(), "transformMatrix")};
	glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(matrix));

	glBindTexture(GL_TEXTURE_2D, texture_sides);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 8);

	glBindTexture(GL_TEXTURE_2D, texture_bottom);
	glDrawArrays(GL_TRIANGLES, 3 * 8, 3 * 2);

	glBindTexture(GL_TEXTURE_2D, texture_top);
	glDrawArrays(GL_TRIANGLES, 3 * 10, 3 * 2);
}
