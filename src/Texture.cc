#define GL_GLEXT_PROTOTYPES

#include "Texture.h"
#include "stb_image.h"
#include <SFML/OpenGL.hpp>
#include <iostream>

using namespace std;

std::map<string, unsigned int> Texture::textures{};

unsigned int Texture::load(string const& filepath)
{
	if(textures.find(filepath) == textures.end())
	{
		stbi_set_flip_vertically_on_load(true);

		int width{};
		int height{};
		int channels{};

		unsigned char *image = stbi_load(filepath.data(), &width, &height, &channels, 0);
		if(!image)
		{
			cout << "Failed loading image" << endl;
			exit(0);
		}


		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 6);
		glGenerateMipmap(GL_TEXTURE_2D);

		textures.emplace(make_pair(filepath, texture));
		stbi_image_free(image);
	}
	return textures.at(filepath);
}
