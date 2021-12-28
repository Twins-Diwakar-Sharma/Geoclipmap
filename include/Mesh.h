#ifndef JALLA
#define JALLA

#include <fstream>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include <iostream>

class Mesh
{
protected:
	std::vector<float> vertexData;
	std::vector<unsigned int> indices;
	unsigned int vao, vbo, ebo;

	void generate();
	
public:
	Mesh();
	Mesh(std::string);
	~Mesh();
	void createPlane();
	void bind();
	void unbind();
	int indicesSize();

};




class ClipMesh : public Mesh
{
protected:

	void createClipMesh(int noOfRings);
	int startSize=16;
	int components=0;
public:
	void generate();
	ClipMesh();
	ClipMesh(int rings);
	~ClipMesh();
	int getStartSize();
	int getSize();
	std::vector<float>& getVertexData();
};

#endif