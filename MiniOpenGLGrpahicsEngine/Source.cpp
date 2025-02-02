#include "glad.h"
#include "glfw3.h"
#include <iostream>
#include "Renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "AOTW_Week3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 1920, 1080);
	glEnable(GL_DEPTH_TEST);

	LoadedObjects ObjectLoader;
	Onetime OneTimeRenderSetup;
	ObjectLoader.LoadObjects();
	ObjectLoader.FindObject("2DBOXFAR")->AddColor(Color(255, 255, 0));
	ObjectLoader.FindObject("WALL2D")->AddColor(Color(0, 0, 255));

	OneTimeRenderSetup.RenderSetup();
	while (!glfwWindowShouldClose(window))
	{
		RenderLoop(ObjectLoader, window, OneTimeRenderSetup);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}