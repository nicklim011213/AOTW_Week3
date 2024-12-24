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

	Object::Object(std::string FileContents, std::string ObjectId)
	{
		this->ObjectId = ObjectId;
		std::stringstream LineReader(FileContents);
		std::string temp, value;
		int beforeindex = 0;
		int afterindex = 0;
		while (std::getline(LineReader, temp))
		{
			while (afterindex != std::string::npos)
			{
				afterindex = temp.find(',', beforeindex + 1);
				value = temp.substr(beforeindex, afterindex - beforeindex);
				beforeindex = afterindex + 1;
			}
		}
	}