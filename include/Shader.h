#ifndef I_SHALL_BE_YOUR_EYES
#define I_SHALL_BE_YOUR_EYES

#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>
#include "Algebra.h"
#include "Camera.h"

#define NOTHING 0
#define GEO 1
#define TESS 2
#define GEOTESS 3

class Shader
{
private:
	std::string path = "Inventory/Shaders/";
	std::map<std::string, int> uniformMap;
	unsigned int programID, vertexID, fragmentID;

public:
	Shader(std::string name);
	Shader(std::string name, int eliteLevel);
	void createShader(unsigned int& shaderID, int shaderType, std::string filename);
	void use();
	void unuse();
	void mapUniform(std::string uniform);
	void mapUniformOLOD(std::string);
	void setUniform(std::string, float, float);
	void setUniform(std::string, int);
	void setUniform(std::string, Vec3&);
	void setUniform(std::string, Vec3&&);
	void setUniform(std::string, Mat4&);
	void setUniform(std::string, Camera&);
	void setUniformOLOD(std::string, int n, int e, int s, int w);
};

#endif
