#include "Objects.h"
#include <string>

	std::shared_ptr<Object> LoadedObjects::FindObject(std::string ID)
	{
		auto it = ObjectStorage.at(ID);
		return it;
	}

	void LoadedObjects::LoadObjects()
	{
		std::filesystem::directory_iterator FilePointer(std::filesystem::current_path());
		for (const std::filesystem::directory_entry Entry : FilePointer)
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
			auto TempObject = std::make_shared<Object>(StringObject, std::string(IDLine));
			std::cerr << "Inserting object with ID: " << IDLine << std::endl;
			ObjectStorage[IDLine] = TempObject;
		}
	}

	Point3D::Point3D(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	Point2D::Point2D(float x, float y)
	{
		this->X = x;
		this->Y = y;
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
							x = std::stof(value);
						}
						else if (i == 1)
						{
							y = std::stof(value);
						}
						if (i == 2)
						{
							z = std::stof(value);
							this->VertexList.push_back(Point3D(x, y, z));
						}
					}				
				}
			}
		}
		this->ColorList = {};
		//CreateCenterCord();
	}

	/*
	void Object::AddTextureCord(int index, float Xtex, float Ytex)
	{
		this->Textures = true;
		int location = 0;
		std::list<Point2D>::iterator it = TexturecordList.begin();
		std::advance(it, index);
		*it = Point2D(Xtex, Ytex);
	}
	*/

	void Object::AddColor(Color basecolor)
	{
		Colors = true;
		for (const auto& index : IndexList)
		{
			ColorList.push_back(basecolor);
		}
	}

	void Object::CreateCenterCord()
	{
		float xmax = VertexList.begin()->X, ymax = VertexList.begin()->Y, zmax = VertexList.begin()->Z, xmin = VertexList.begin()->X, ymin = VertexList.begin()->Y, zmin = VertexList.begin()->Z;
		for (auto Vectors = VertexList.begin(); Vectors != VertexList.end(); Vectors++)
		{
			if (xmax < Vectors->X)
			{
				xmax = Vectors->X;
			}
			else if (xmin > Vectors->X)
			{
				xmin = Vectors->X;
			}

			if (ymax < Vectors->Y)
			{
				ymax = Vectors->Y;
			}
			else if (ymin > Vectors->Y)
			{
				ymin = Vectors->Y;
			}

			if (zmax < Vectors->Z)
			{
				zmax = Vectors->Z;
			}
			else if (zmin > Vectors->Z)
			{
				zmin = Vectors->Z;
			}
		}
		float x = 0, y = 0, z = 0;
		x = (abs(xmax) - abs(xmin)) / 2;
		y = (abs(ymax) - abs(ymin)) / 2;
		z = (abs(zmax) - abs(zmin)) / 2;
		CenterPoint = Point3D(x,y,z);
	}