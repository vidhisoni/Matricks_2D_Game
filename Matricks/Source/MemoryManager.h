/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

#define MAX_COMPONENTS 200
#define MAX_GAMEOBJECTS 200

enum GameObjectType;
enum ComponentType;
class GameObject;
class Component;

class MemoryManager
{
	static MemoryManager* pObject;
	MemoryManager() { }
public:
	static MemoryManager* CreateInstance();
	inline static MemoryManager* GetInstance() { return pObject; }

	void Init();
	void Close();
	GameObject* NewGameObject(GameObjectType Type);
	void FreeGameObject(GameObject* FreeObject);

	Component* NewComponent(ComponentType CompType);
	void FreeComponent(Component* FreeComponent);

	inline int ActiveGameObjects() { return MAX_GAMEOBJECTS - GameObjectMemory.size(); }
	inline int GameObjectsInMemory() { return GameObjectMemory.size(); }
	inline int ActiveComponents(unsigned int comp) { return MAX_COMPONENTS - ComponentMemory[comp].size(); }
	inline int ComponentsInMemory(unsigned int comp) { return ComponentMemory[comp].size(); }

private:
	std::map<unsigned int, std::list<Component*>> ComponentMemory;
	std::list<GameObject*> GameObjectMemory;
	std::vector<unsigned int> ExcludeComponents;
};
