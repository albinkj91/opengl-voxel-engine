#ifndef GRASS_H
#define GRASS_H

#include "Voxel.h"

class Grass : public Voxel
{
public:
	Grass(glm::vec3 const& position,
		Program const& program,
		unsigned int const tx_top,
		unsigned int const tx_sides,
		unsigned int const tx_bottom);

	void render() const override;
private:
	unsigned int texture_top;
	unsigned int texture_sides;
	unsigned int texture_bottom;
};

#endif
