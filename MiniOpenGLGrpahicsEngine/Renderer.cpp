#include "Renderer.h"

void RenderLoopSetup(LoadedObjects &RenderObjects, GLFWwindow* window)
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.8f, 0.0f, 0.8f, 1.0);\n"
		"}\n\0";

	const char* vertexShaderSourceColor = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 acolor;\n"
		"out vec3 acolor_out;"
		"void main()\n"
		"{\n"
		"	acolor_out = acolor;\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSourceColor = "#version 330 core\n"
		"in vec3 acolor_out;"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(acolor_out, 1.0);\n"
		"}\n\0";

	// Non Color Shaders
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

	// Color Shaders
	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &vertexShaderSourceColor, NULL);
	glCompileShader(vertexShaderColor);
	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &fragmentShaderSourceColor, NULL);
	glCompileShader(fragmentShaderColor);
	unsigned int shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderColor);

	std::vector<float> VertexData, VertexDataColor;
	for (const auto& [ID, OBJ] : RenderObjects.ObjectStorage)
	{
		if (OBJ->Colors == true)
		{
			int colorindex = 0;
			auto ColorVal = OBJ->ColorList[colorindex];
			for (const auto Vert : OBJ->VertexList)
			{
				VertexDataColor.push_back(Vert.X);
				VertexDataColor.push_back(Vert.Y);
				VertexDataColor.push_back(Vert.Z);
				VertexDataColor.push_back(ColorVal.R);
				VertexDataColor.push_back(ColorVal.G);
				VertexDataColor.push_back(ColorVal.B);
				colorindex++;
			}
		}
		else
		{
			for (const auto Vert : OBJ->VertexList)
			{
				VertexData.push_back(Vert.X);
				VertexData.push_back(Vert.Y);
				VertexData.push_back(Vert.Z);
			}
		}
	}

	std::vector<int> IndexData, IndexDataColor;
	for (const auto& [ID, OBJ] : RenderObjects.ObjectStorage)
	{
		if (OBJ->Colors)
		{
			for (auto IndexItter : OBJ->IndexList)
			{
				IndexDataColor.push_back(IndexItter);
			}
		}
		else
		{
			for (auto IndexItter : OBJ->IndexList)
			{
				IndexData.push_back(IndexItter);
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
	glGenBuffers(1, &EBOColor);

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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOColor);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexDataColor.size() * sizeof(int), IndexDataColor.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Draw Commands
	glClearColor(0.51f, 0.78f, 0.89f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(shaderProgramColor);
	glBindVertexArray(VAOColor);
	glDrawElements(GL_TRIANGLES, IndexDataColor.size(), GL_UNSIGNED_INT, 0);
}
