#ifndef OBJECTMODULE
#define OBJECTMODULE

#include <iostream>
#include <list>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>

class Point3D
{
	public:
		float X = 0;
		float Y = 0;
		float Z = 0;

		Point3D(float, float, float);

		Point3D() {};
};

class Point2D
{
		float X = 0;
		float Y = 0;

		Point2D() {};

	public: 
		Point2D(float x, float y);
};

class Color
{
	public:
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
	std::string ObjectId;
	std::vector<Point3D> VertexList = {};
	std::vector<int> IndexList = {};
	bool Textures = false;
	bool Colors = false;
	std::vector<Point2D> TexturecordList = {};
	std::vector<Color> ColorList = {};
	Point3D CenterPoint;

	Object(std::string FileContents, std::string ObjectId);

	//void AddTextureCord(int index, float Xtex, float Ytex);

	void AddColor(Color basecolor);

	void CreateCenterCord();
};

class LoadedObjects
{
public:
	std::unordered_map<std::string, std::shared_ptr<Object>> ObjectStorage;

	std::shared_ptr<Object> FindObject(std::string ID);
	void LoadObjects();
};

#endif // ! OBJECTMODULE