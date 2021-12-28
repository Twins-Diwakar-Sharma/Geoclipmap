#ifndef EENDHAN
#define EENDHAN

#include "Renderer.h"
#include "Window.h"
#include "Object.h"
#include "Camera.h"
#include <time.h>
#include <thread>

class Engine
{
private:
	Window* w = nullptr;
	std::thread* gameloop;
	Renderer* r;
	Object* o;
	Camera* cam;
	double fps = 60.0f;
	double msPerUpdate = 1000 / fps;
	float translateForward, translateSide;
	float transVal = 0.05f;
	float rotx, roty;
	struct Sun {
		Vec3 lightColor, lightDir;
	} sun;
	std::vector<Object> trees, presets;

	Texture* heightmap, *normalmap;
	ClipmapRenderer* clipmapRenderer;
	Clipmap* clipmap;
	GeoClipmap* geoClipmap;

	bool letUpdate=false;

	void init();
	void pump();
	void input();
	void update();
	void render(double);



public:
	Engine();
	
};

#endif

