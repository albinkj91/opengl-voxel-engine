#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <map>

class Texture
{
public:
	static unsigned int load(std::string const& filepath);
private:
	Texture();
	static std::map<std::string, unsigned int> textures;
};

#endif
