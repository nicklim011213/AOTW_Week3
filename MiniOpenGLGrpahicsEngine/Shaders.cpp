#include "Shaders.h"

	Shader::Shader(std::string Filepath, char ShaderType)
	{
		std::string Filename = "";
		std::ifstream File(Filepath);
		if (!File.good())
		{
			std::cerr << "Error with file at: " << Filepath << "\n";
			return;
		}
		int FilenameStart = Filepath.find_last_of('\\') + 1;
		Filename = Filepath.substr(FilenameStart);
		if (Filename.find(".vs") != std::string::npos)
		{
			ShaderType = 'v';
		}
		else if (Filename.find(".fs") != std::string::npos)
		{
			ShaderType = 'f';
		}
		else
		{
			std::cerr << "Unable to determine shader type. (Does it end with .vs or .fs?) \n";
		}

		std::string temp = "";
		while (std::getline(File, temp))
		{
			Code += temp;
		}
	}