#include "Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void RenderLoop(LoadedObjects& RenderObjects, GLFWwindow* window, Onetime OneTime)
{
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main()\n"
		"{\n"
		"   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
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
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"out vec3 acolor_out;"
		"void main()\n"
		"{\n"
		"	acolor_out = acolor;\n"
		"   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
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

	glUseProgram(shaderProgramColor);
	unsigned int modelLocColor = glGetUniformLocation(shaderProgramColor, "model");
	unsigned int viewLocColor = glGetUniformLocation(shaderProgramColor, "view");
	unsigned int projLocColor = glGetUniformLocation(shaderProgramColor, "projection");
	glUseProgram(shaderProgram);
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

	glUseProgram(shaderProgramColor);
	glUniformMatrix4fv(modelLocColor, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLocColor , 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLocColor, 1, GL_FALSE, glm::value_ptr(projection));

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderColor);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::vector<int> IndexData, IndexDataColor;
	std::vector<float> VertexData, VertexDataColor;
	int vertexOffsetColor = 0, vertexOffset = 0;
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
			for (auto IndexItter : OBJ->IndexList)
			{
				IndexDataColor.push_back(IndexItter + vertexOffsetColor);
			}
			vertexOffsetColor += OBJ->VertexList.size();
		}
		else
		{
			for (const auto Vert : OBJ->VertexList)
			{
				VertexData.push_back(Vert.X);
				VertexData.push_back(Vert.Y);
				VertexData.push_back(Vert.Z);
			}
			for (auto IndexItter : OBJ->IndexList)
			{
				IndexData.push_back(IndexItter + vertexOffset);
			}
			vertexOffset += OBJ->VertexList.size();
		}
	}

	// Non Color Objects
	glBindVertexArray(OneTime.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, OneTime.VBO);
	glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(float), VertexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OneTime.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexData.size() * sizeof(int), IndexData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Color Objects
	glBindVertexArray(OneTime.VAOColor);
	glBindBuffer(GL_ARRAY_BUFFER, OneTime.VBOColor);
	glBufferData(GL_ARRAY_BUFFER, VertexDataColor.size() * sizeof(float), VertexDataColor.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, OneTime.EBOColor);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexDataColor.size() * sizeof(int), IndexDataColor.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Draw Commands
	glClearColor(0.51f, 0.78f, 0.89f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(OneTime.VAO);
	//glDrawElements(GL_TRIANGLES, IndexData.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(shaderProgramColor);
	glBindVertexArray(OneTime.VAOColor);
	glDrawElements(GL_TRIANGLES, IndexDataColor.size(), GL_UNSIGNED_INT, 0);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgramColor);
}

