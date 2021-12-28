#ifndef KHIDKI
#define KHIDKI

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
private:
	GLFWwindow* win;
	int width = 800, height = 600;
	void static resize(GLFWwindow* w, int width, int height);
	float senstivity = 0.05f;

	bool keyPress[4] = { false };

public:
	Window();
	bool close();
	void terminate();
	void swap();
	float getAspect();
	void handleKey(float& t, float& s, float val);
	void handleMouse(float& rx, float& ry);
	void handleTess(int& iLOD, int& oLOD);
	void handleUpdate(bool& letUpdate);
};

#endif