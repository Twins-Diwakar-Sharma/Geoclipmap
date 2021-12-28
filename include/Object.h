#ifndef WASTU
#define WASTU

#include "Texture.h"
#include "Mesh.h"
#include "Algebra.h"
#include <fstream>
#include <sstream>
#include "Camera.h"

class Object
{
private:
	Mesh* mesh;
	Texture* tex;
	Mat4 transform;

	
	void syncTrans();

public:
	Vec3 rotation, scale, position;
	Object(Mesh* m, Texture* t);
	void bind();
	void unbind();
	unsigned int getTexID();
	int meshIndSize();
	Mat4& getTransform();
	void setPosition(float x, float y, float z);
	void setPosition(Vec3& p);
	void setPosition(Vec3&& p);
	void setRotation(float x, float y, float z);
	void translate(float dx, float dy, float dz);
	void rotate(float rx, float ry, float rz);
	void setScale(float x, float y, float z);
};


class Clipmap
{
private:
	Vec3 position;
	ClipMesh* mesh;
	int rings=4;
public:
	Clipmap();
	~Clipmap();
	void bindMesh();
	void unbindMesh();
	int getNoOfRings();
	Vec3& getPosition();

	void update(Vec3& campos);
	int getMeshSize();
};

class Mipmap
{
private:
	Vec3 position;
	ClipMesh* mesh;


public:
	Mipmap* parent = nullptr;
	int stepX = 0, stepZ = 0;
	int stepSize = 1;
	Mipmap();
	~Mipmap();
	void bindMesh();
	void unbindMesh();
	Vec3& getPosition();

	void update(int dirX, int dirZ);
	int getMeshSize();
	ClipMesh* getMesh();

};

class GeoClipmap
{
private:
	int rings=0;
	int startSize = 32; // = (lod in tcs * stepsize of mipmaps[0]) otherwise wobble
public:
	Mipmap** mipmaps;
	GeoClipmap(int rings);
	~GeoClipmap();
	void update(Vec3& campos);
	int getNoOfRings();

};


#endif

