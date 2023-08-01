#define GL_GLEXT_PROTOTYPES

#include "Grass.h"
#include <SFML/OpenGL.hpp>
#include <iostream>

Grass::Grass(glm::vec3 pos,
		unsigned const int tx_top,
		unsigned const int tx_sides,
		unsigned const int tx_bottom)
	:Voxel{pos}, texture_top{tx_top}, texture_sides{tx_sides}, texture_bottom{tx_bottom}
{ }

void Grass::render() const
{
	glBindTexture(GL_TEXTURE_2D, texture_sides);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 8);

	glBindTexture(GL_TEXTURE_2D, texture_bottom);
	glDrawArrays(GL_TRIANGLES, 3 * 8, 3 * 2);

	glBindTexture(GL_TEXTURE_2D, texture_top);
	glDrawArrays(GL_TRIANGLES, 3 * 10, 3 * 2);
}
