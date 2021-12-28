#ifndef RAIN_DEER
#define RAIN_DEER

#include "Shader.h"
#include "Object.h"
#include "Algebra.h"
#include "Camera.h"

extern Mat4 projection;
void setProjection(float fov, float near, float far, float as);

class Renderer
{
private:
	Shader* genericShader;
	
public:
	Renderer();
	~Renderer();
    void render(Object& o, Camera& c, Vec3& lightColor, Vec3& lightDir);
};

class ClipmapRenderer
{
private:
	Shader* shader;
public:
	ClipmapRenderer();
	~ClipmapRenderer();
	void render(Clipmap& clipmap, Texture& heightMap, Texture& normalMap, Camera& camera, Vec3& lightColor, Vec3& lightDir);
	void render(GeoClipmap& geo, Texture& heightMap, Texture& normalMap, Camera& camera, Vec3& lightColor, Vec3& lightDir);
};

#endif