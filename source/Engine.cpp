#include "Engine.h"

Engine::Engine()
{
	gameloop = new std::thread(&Engine::pump, this);
	gameloop->join();
}

void Engine::init()
{
	sun.lightColor = { 1,0.7,0.7 };
	sun.lightDir = { 0,-1,0 };

	w = new Window();
	setProjection(60, 0.1f, 1000, w->getAspect());
	r = new Renderer();
	cam = new Camera();
	cam->setRotation(0, 0, 0);
	cam->setPosition(0,5,0);

	Mesh* objectMesh = new Mesh("alpine");
	Texture* objectTexture = new Texture("alpine.png");
	o = new Object(objectMesh,objectTexture);
	o->setPosition(0, 0, 0);

	heightmap = new Texture("heightmap.jpg");
	normalmap = new Texture("normalmap.jpg");
	clipmapRenderer = new ClipmapRenderer();
	clipmap = new Clipmap();

	geoClipmap = new GeoClipmap(3);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);


}

void Engine::pump()
{
	
	init();
	clock_t pre = clock();
	clock_t cur = clock();
	double elapsed = 0;
	double lag = 0;

	int frameCount=0;
	double previousTime = glfwGetTime();
	while (!w->close())
	{
		elapsed = ((double)cur - (double)pre) * 1000 / CLOCKS_PER_SEC;
	
		lag += elapsed;

		double currentTime = glfwGetTime();
		frameCount++;

		if (currentTime - previousTime >= 1.0)
		{
			//std::cout << frameCount << "  "  << std::flush;
			frameCount = 0;
			previousTime = currentTime;
		}

		pre = clock();
		input();
		while (lag > msPerUpdate)
		{
			update();
			lag -= msPerUpdate;
		}
		render(lag / msPerUpdate);
		cur = clock();
	}
	w->terminate();
}

void Engine::input()
{
	w->handleKey(translateForward, translateSide, transVal);
	w->handleMouse(rotx, roty);
	w->handleUpdate(letUpdate);
}

void Engine::update()
{
	cam->spinIt(rotx, roty, 0);
	rotx = 0;	roty = 0;
	cam->move(translateForward, translateSide);

	//clipmap->update(cam->position);

	if(letUpdate)
		geoClipmap->update(cam->position);

	translateForward = 0; translateSide = 0;

}

void Engine::render(double f)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//r->render(*o, *cam, sun.lightColor, sun.lightDir);
//	clipmapRenderer->render(*clipmap, *heightmap, *normalmap, *cam, sun.lightColor, sun.lightDir);
	clipmapRenderer->render(*geoClipmap, *heightmap, *normalmap, *cam, sun.lightColor, sun.lightDir);
	w->swap();
}