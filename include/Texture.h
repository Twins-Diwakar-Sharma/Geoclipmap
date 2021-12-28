#ifndef TEXT_ME
#define TEXT_ME

#include <string>
#include <glad/glad.h>
#include <iostream>

class Texture
{
private:
	std::string path = "Inventory/Textures/";
	int width, height, channels;
	unsigned char* data;
	unsigned int texture;

public:
	Texture(std::string filename);
	unsigned int getID();
};

#endif
