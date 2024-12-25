#include "Objects.h"
#include <string>

	LoadedObjects* LoadedObjects::Instance = nullptr;

	Object LoadedObjects::FindObject(std::string ID)
	{
		return *Instance->ObjectStorage.find(ID)->second;
	}

	void LoadedObjects::LoadObjects()
	{
		std::filesystem::directory_iterator FilePointer(std::filesystem::current_path());
		for (const auto Entry : FilePointer)
		{
			if (Entry.path().string().find(".OBX") == std::string::npos)
			{
				continue;
			}
			std::ifstream File(Entry.path());
			std::string temp, StringObject, IDLine;
			bool FirstLine = true;
			while (std::getline(File, temp))
			{
				if (FirstLine)
				{
					IDLine += temp;
					FirstLine = false;
				}
				else
				{
					StringObject += temp + "\n";
				}
			}
			FirstLine = true;
			Object* TempObject = new Object(StringObject, IDLine);
			ObjectStorage[IDLine] = TempObject;
			TempObject = nullptr;
			// Note we still have to delete the objects in the map to avoid mem leaks
		}
	}

	Point3D::Point3D(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	Object::Object(std::string FileContents, std::string ObjectId)
	{
		this->ObjectId = ObjectId;
		std::stringstream LineReader(FileContents);
		std::string temp, value;
		int beforeindex = 10;
		int afterindex = 0;
		while (std::getline(LineReader, temp))
		{
			beforeindex = 10;
			afterindex = 0;
			while (afterindex != std::string::npos)
			{
				if (temp.find("IndexList:") != std::string::npos)
				{
					afterindex = temp.find(',', beforeindex + 1);
					value = temp.substr(beforeindex, afterindex - beforeindex);
					beforeindex = afterindex + 1;
					this->IndexList.push_back(std::stoi(value));
				}
				else if (temp.find("VertsList:") != std::string::npos)
				{
					float x = 0;
					float y = 0;
					float z = 0;
					for (int i = 0; i != 3; ++i)
					{
						afterindex = temp.find(',', beforeindex + 1);
						value = temp.substr(beforeindex, afterindex - beforeindex);
						beforeindex = afterindex + 1;
						if (i == 0)
						{
							x = std::stoi(value);
						}
						else if (i == 1)
						{
							y = std::stoi(value);
						}
						if (i == 2)
						{
							z = std::stoi(value);
							this->VertexList.push_back(Point3D(x, y, z));
						}
					}				
				}
			}
		}
	}