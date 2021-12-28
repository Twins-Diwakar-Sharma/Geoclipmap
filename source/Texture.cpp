#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(std::string filename)
{
	std::string imgext = filename.substr(filename.length() - 3, 3);
	GLint format;
	if (imgext.compare("jpg") == 0 || imgext.compare("peg") == 0)
		format = GL_RGB;
	else if (imgext.compare("png") == 0)
		format = GL_RGBA;
	else
		std::cerr << "\aUnkown image format : " << filename << std::endl;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::string wholeTex = path + filename;
	std::cout << wholeTex << std::endl;
	const char* cwholeTex = wholeTex.c_str();
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(cwholeTex, &width, &height, &channels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "\a Failed to load Image data for : " << filename << std::endl;
	}
	stbi_image_free(data);
}

unsigned int Texture::getID()
{
	return texture;
}
