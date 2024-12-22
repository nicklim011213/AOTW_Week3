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
				StringObject += temp + "\n";
			}
			FirstLine = true;
			Object* TempObject = new Object(StringObject, IDLine);
			ObjectStorage[IDLine] = TempObject;
			TempObject = nullptr;
			// Note we still have to delete the objects in the map to avoid mem leaks
		}
	}