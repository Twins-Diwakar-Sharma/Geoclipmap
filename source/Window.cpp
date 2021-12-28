
#include "Window.h"

Window::Window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	win = glfwCreateWindow(width, height, "TwilightZone", NULL, NULL);
	if (win == NULL)
	{
		std::cerr << " Khidki toot gyi " << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(win);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initalize glad" << std::endl;
	}

	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(win, resize);
}

void Window::resize(GLFWwindow* w, int width, int height)
{
	glViewport(0, 0, width, height);
}

bool Window::close()
{
	return glfwWindowShouldClose(win);
}

void Window::terminate()
{
	glfwTerminate();
}

void Window::swap()
{
	glfwSwapBuffers(win);
}

float Window::getAspect()
{
	return (float)width/height;
}


void Window::handleKey(float& t, float& s, float val)
{
	if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		t += val;
	else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		t -= val;

	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		s -= val;
	else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		s += val;

	glfwPollEvents();
}

void Window::handleMouse(float& rx, float& ry)
{
	double x, y;
	glfwGetCursorPos(win, &x, &y);
	ry += -(x - width / 2) * senstivity; 
	rx += -(y - height / 2) * senstivity;
	
	glfwSetCursorPos(win, (double)width / 2, (double)height / 2);
}

void Window::handleTess(int& iLOD, int& oLOD)
{
	if (!keyPress[0] && glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS)
	{
		keyPress[0] = true;
		iLOD++;
	}
	if (keyPress[0] && glfwGetKey(win, GLFW_KEY_UP) == GLFW_RELEASE)
	{
		keyPress[0] = false;
	}

	if (!keyPress[1] && glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		keyPress[1] = true;
		iLOD--;
		if (iLOD < 1)
			iLOD = 1;
	}
	if (keyPress[1] && glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_RELEASE)
	{
		keyPress[1] = false;
	}

	if (!keyPress[2] && glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		keyPress[2] = true;
		oLOD++;
	}
	if (keyPress[2] && glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_RELEASE)
	{
		keyPress[2] = false;
	}

	if (!keyPress[3] && glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		keyPress[3] = true;
		oLOD--;
		if (oLOD < 1)
			oLOD = 1;
	}
	if (keyPress[3] && glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_RELEASE)
	{
		keyPress[3] = false;
	}
}


void Window::handleUpdate(bool& update)
{
	if(glfwGetKey(win, GLFW_KEY_U) == GLFW_PRESS)
	{
		if(glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			update = false;
		else
			update = true;
	}
}