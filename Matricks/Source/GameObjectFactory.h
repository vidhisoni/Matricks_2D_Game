/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class GameObjectFactory
{
	static GameObjectFactory* pObject;
	std::map<GameObjectType, rapidjson::Document*> mArchetypes;
	GameObjectFactory() { }

public:
	static GameObjectFactory* CreateInstance() { pObject = new GameObjectFactory(); return pObject; }
	inline static GameObjectFactory* GetInstance() { return pObject; }

	GameObject* LoadObject(const char* pFileName);
	GameObject* GetArcheType(GameObjectType type);
	GameObject* LoadObject(rapidjson::Value::Object root, std::string fname);
	inline GameObjectType GetGameObjectEnum(std::string type) { return mGameObjectMap[type]; }
	
	void Init();
	void Close();

//private:
	std::map<std::string, GameObjectType> mGameObjectMap;
	std::map<std::string, ComponentType> mComponentEnum;

};