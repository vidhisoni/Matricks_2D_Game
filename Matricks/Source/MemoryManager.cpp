/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"

MemoryManager* MemoryManager::pObject = nullptr;

MemoryManager* MemoryManager::CreateInstance()
{

	pObject = new MemoryManager();
	return pObject;
}

void MemoryManager::Init()
{
	std::ifstream file("Resources/Data/MemoryManagerConfig.json");
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	rapidjson::Document doc;
	doc.Parse(contents.c_str());
	
	rapidjson::Value::Array exclusions = doc["ExcludeComponents"].GetArray();
	for (unsigned int i = 0; i < exclusions.Size(); ++i)
	{
		ExcludeComponents.push_back(exclusions[i].GetUint());
	}

	for (unsigned int i = 0; i < MAX_GAMEOBJECTS; ++i)
	{
		GameObject* temp = new GameObject();
		GameObjectMemory.push_back(temp);
	}

	auto comp = GameObjectManager::GetInstance()->mComponentsMap.begin();
	for (unsigned int i = 0; i < GameObjectManager::GetInstance()->mComponentsMap.size(); ++i)
	{
		std::list<Component*> templist;
		if (std::find(ExcludeComponents.begin(), ExcludeComponents.end(), comp->first) == ExcludeComponents.end())
		{
			for (unsigned int j = 0; j < MAX_COMPONENTS; ++j)
			{
				Component* temp = comp->second->Create();
				templist.push_back(temp);
			}
		}
		else
		{
			Component* temp = comp->second->Create();
			templist.push_back(temp);
		}
		ComponentMemory[comp->first] = templist;
		comp++;
	}
}

void MemoryManager::Close()
{
	for (auto go : GameObjectMemory)
	{
		delete go;
	}
	GameObjectMemory.clear();

	for (auto com : ComponentMemory)
	{
		for (auto comp : com.second)
		{
			delete comp;
		}
		com.second.clear();
	}
	ComponentMemory.clear();

	delete pObject;
}

GameObject* MemoryManager::NewGameObject(GameObjectType Type)
{
	if (GameObjectMemory.empty())
	{
		LogError("[MEMORY] No GameObjects to Allocate");
	}

	GameObject* NewObject = GameObjectMemory.front();
	GameObjectMemory.pop_front();
	NewObject->SetType(Type);
	return NewObject;
}

void MemoryManager::FreeGameObject(GameObject* FreeObject)
{
	FreeObject->Clear();
	GameObjectMemory.push_back(FreeObject);
}

Component* MemoryManager::NewComponent(ComponentType CompType)
{
	if (ComponentMemory[CompType].empty())
	{
		LogError("[MEMORY] No Components to Allocate");
	}

	Component* NewComp = ComponentMemory[CompType].front();
	ComponentMemory[CompType].pop_front();
	return NewComp;
}

void MemoryManager::FreeComponent(Component* FreeComponent)
{
	ComponentType CompType = FreeComponent->GetType();
	FreeComponent->Clear();
	ComponentMemory[CompType].push_back(FreeComponent);
}

