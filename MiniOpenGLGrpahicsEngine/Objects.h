#ifndef OBJECTMODULE
#define OBJECTMODULE

#include <iostream>
#include <list>
#include <map>
#include <filesystem>
#include <fstream>

class Point3D
{
	float X = 0;
	float Y = 0;
	float Z = 0;
};

class Point2D
{
	float X = 0;
	float Y = 0;
};

class Color
{
	int R = 255;
	int G = 255;
	int B = 255;
	int A = 255;

	Color(int R, int G, int B)
	{
		this->R = R;
		this->G = G;
		this->B = B;
		A = 255;

		if (R > 255 || R < 0)
		{
			R = 255;
			std::cerr << "Red value out of range set to 255\n";
		}
		if (G > 255 || G < 0)
		{
			G = 255;
			std::cerr << "Green value out of range set to 255\n";
		}
		if (B > 255 || B < 0)
		{
			B = 255;
			std::cerr << "Blue value out of range set to 255\n";
		}
	}
};

class Object
{
public:
	std::string ObjectId = "UniqueObjectId";
	std::list<Point3D> VertexList;
	std::list<int> IndexList;
	std::list<Point2D> TexturecordList;
	std::list<Color> ColorList;
	Point3D CenterPoint;

	Object(std::string FileContents, std::string ObjectId)
	{
		this->ObjectId = ObjectId;
	}
};

class LoadedObjects
{
public:

	static LoadedObjects* Instance;
	std::map<std::string, Object*> ObjectStorage;

	LoadedObjects()
	{
		if (Instance == nullptr)
		{
			Instance = this;
		}
		else
		{
			std::cerr << "Error attempted a second creation of singleton LoadedObjects.\n";
		}
	}

	Object FindObject(std::string ID);
	void LoadObjects();

	LoadedObjects(LoadedObjects& Copy) {};
	void operator=(LoadedObjects& Arg) {};
};

#endif // ! OBJECTMODULE