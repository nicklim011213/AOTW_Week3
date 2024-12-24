#ifndef SHADERMODULE
#define SHADERMODULE

#include <iostream>
#include <fstream>
#include <string>

class Shader
{

public:
	std::string Name;
	char ShaderType = 'U';
	std::string Code;
	int ShaderId = -1;

	Shader(std::string Filepath, char ShaderType);

};


#endif // ! SHADERMODULE