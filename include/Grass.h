#ifndef GRASS_H
#define GRASS_H

#include "Voxel.h"

class Grass : public Voxel
{
public:
	Grass(glm::vec3 pos,
		unsigned const int tx_top,
		unsigned const int tx_sides,
		unsigned const int tx_bottom);

	void render() const override;
private:
	unsigned int texture_top;
	unsigned int texture_sides;
	unsigned int texture_bottom;
};

#endif
