#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:
	static unsigned int load(std::string const& filepath);
private:
	Texture();
};

#endif
