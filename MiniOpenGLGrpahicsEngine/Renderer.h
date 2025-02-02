#ifndef RENDERER
#define RENDERER
#include "Objects.h"
#include "glad.h"
#include "glfw3.h"
#include <vector>

class Onetime
{
public:
		unsigned int VAO, VAOColor, VBO, EBO, VBOColor, EBOColor;
	void RenderSetup()
	{
		glGenVertexArrays(1, &VAO);
		glGenVertexArrays(1, &VAOColor);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &VBOColor);
		glGenBuffers(1, &EBOColor);
	}
};

void RenderLoop(LoadedObjects &RenderObjects, GLFWwindow* window, Onetime OneTime);
#endif
