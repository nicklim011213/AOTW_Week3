#include "Renderer.h"

void RenderLoopSetup(LoadedObjects &RenderObjects, GLFWwindow* window)
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 acolor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.8f, 0.0f, 0.8f, 1f);\n"
		"}\n\0";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::vector<float> VertexData;
	for (auto itter = RenderObjects.ObjectStorage.begin(); itter != RenderObjects.ObjectStorage.end(); ++itter)
	{
		for (auto VertItter = itter->second->VertexList.begin(); VertItter != itter->second->VertexList.end(); ++VertItter)
		{
			VertexData.push_back(VertItter->X);
			VertexData.push_back(VertItter->Y);
			VertexData.push_back(VertItter->Z);
		}
	}

	std::vector<int> IndexData, IndexDataColor;
	for (auto itter = RenderObjects.ObjectStorage.begin(); itter != RenderObjects.ObjectStorage.end(); ++itter)
	{
		if (itter->second->Colors)
		{
			for (auto IndexItter = itter->second->IndexList.begin(); IndexItter != itter->second->IndexList.end(); ++IndexItter)
			{
				IndexDataColor.push_back(*IndexItter);
			}
		}
		else
		{
			for (auto IndexItter = itter->second->IndexList.begin(); IndexItter != itter->second->IndexList.end(); ++IndexItter)
			{
				IndexData.push_back(*IndexItter);
			}
		}
	}

	unsigned int VAO, VBO, EBO;
	unsigned int VAOColor, VBOColor, EBOColor;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAOColor);
	glGenBuffers(1, &VBOColor);
	glGenBuffers(1, &EBO);

	// Non Color Objects
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(float), VertexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexData.size() * sizeof(int), IndexData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Color Objects
	glBindVertexArray(VAOColor);
	glBindBuffer(GL_ARRAY_BUFFER, VBOColor);
	glBufferData(GL_ARRAY_BUFFER, VertexDataColor.size() * sizeof(float), VertexDataColor.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexData.size() * sizeof(int), IndexData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 6, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Draw Commands
	glClearColor(0.51f, 0.78f, 0.89f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}
