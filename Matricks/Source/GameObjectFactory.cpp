/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "ParticleSystem.h"


GameObjectFactory* GameObjectFactory::pObject = nullptr;

void GameObjectFactory::Init()
{
	//Component Enum map
	mComponentEnum["Animator"] = ANIMATOR;
	mComponentEnum["Transform"] = TRANSFORM;
	mComponentEnum["Sprite"] = SPRITE;
	mComponentEnum["Controller"] = CONTROLLER;
	mComponentEnum["Body"] = BODY;
	mComponentEnum["Audio"] = AUDIO;
	mComponentEnum["Particle"] = PARTICLE;
	mComponentEnum["State"] = STATE;
	mComponentEnum["PlayerState"] = PLAYERSTATE;
	mComponentEnum["Parallax"] = PARALLAX;
	mComponentEnum["Subscription"] = SUBSCRIPTION;
	mComponentEnum["Text"] = TEXT;
	mComponentEnum["Light"] = LIGHT;
	mComponentEnum["Cloud"] = CLOUD;
	mComponentEnum["Button"] = BUTTON;
	mComponentEnum["Script"] = SCRIPT;
	mComponentEnum["LinearAI"] = LINERAI;
	mComponentEnum["Alarm"] = ALARMCOMPONENT;
	mComponentEnum["Collider"] = COLLIDER;
	mComponentEnum["Prompt"] = PROMPT;
	mComponentEnum["Trigger"] = COMP_TRIGGER;
	mComponentEnum["Story"] = STORY;
	mComponentEnum["MenuControl"] = MENUCONTROL;
	mComponentEnum["TestAI"] = TESTAI;
	mComponentEnum["Levels"] = LEVEL;
	//Component Enum map

	//GameObject Enum map
	mGameObjectMap["NoObject"] = NOOBJECT;
	mGameObjectMap["Player"] = PLAYER;
	mGameObjectMap["Platform"] = PLATFORM;
	mGameObjectMap["Enemy"] = ENEMY;
	mGameObjectMap["Background"] = BACKGROUND;
	mGameObjectMap["Numbox"] = NUMBOX;
	mGameObjectMap["IDoor"] = IDOOR;
	mGameObjectMap["Button"] = BUTTONOBJ;
	mGameObjectMap["Trap"] = TRAP;
	mGameObjectMap["FinalDoor"] = FINALDOOR;
	mGameObjectMap["InvisiPlatform"] = INVISIPLATFORM;
	mGameObjectMap["Alarm"] = ALARM;
	mGameObjectMap["Bouncy"] = BOUNCY;
	mGameObjectMap["AIEnemy"] = AI_ENEMY;
	mGameObjectMap["StartDoor"] = STARTDOOR;
	mGameObjectMap["Prompt"] = PROMPTER;
	mGameObjectMap["Prompt_Trigger"] = P_TRIGGER;
	mGameObjectMap["Box"] = BOX;
	mGameObjectMap["Charge"] = CHARGE;
	mGameObjectMap["Cloud"] = NUMCLOUD;
	mGameObjectMap["Lift"] = LIFT;
	mGameObjectMap["Value"] = VALUE;
	mGameObjectMap["BoxTrigger"] = BOXTRIGGER;
	mGameObjectMap["OutBound"] = OUTBOUND;
	mGameObjectMap["AIbot"] = TESTAIBOT;
	mGameObjectMap["Portal"] = PORTAL;

	//GameObject Enum map

	//Loading Archetypes
	std::string fullPath = "Resources/Data/Archetypes.json";
	std::ifstream file(fullPath);
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	rapidjson::Document root;
	root.Parse(contents.c_str());

	rapidjson::Value::Array prefabs = root["Prefabs"].GetArray();
	std::string type;
	std::string FileName;
	for (unsigned int i = 0; i < prefabs.Size(); ++i)
	{
		rapidjson::Value::Object obj = prefabs[i].GetObject();
		type = obj["Type"].GetString();
		FileName = obj["File"].GetString();

		std::string Path = "Resources/Data/";
		Path += FileName;
		std::ifstream file(Path);
		std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		rapidjson::Document *PrefabDoc = new rapidjson::Document();
		PrefabDoc->Parse(contents.c_str());
		mArchetypes[mGameObjectMap[type]] = PrefabDoc;
	}
}


GameObject* GameObjectFactory::LoadObject(const char *pFileName)
{
	GameObject *pNewGameObject = nullptr;

	std::string fullPath = "Resources/Data/";
	fullPath += pFileName;
	std::ifstream file(fullPath);
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
	rapidjson::Document root;
	root.Parse(contents.c_str());

	if (root.IsObject()) {
		pNewGameObject = LoadObject(root.GetObject(), pFileName);
	}
	return pNewGameObject;

}

GameObject* GameObjectFactory::LoadObject(rapidjson::Value::Object root, std::string fname)
{
	GameObject *pNewGameObject = nullptr;
	Component *pNewComponent = nullptr;
	
	std::string type = root["Type"].GetString();

	pNewGameObject = MEMORY_MANAGER->NewGameObject(mGameObjectMap[type]);
	pNewGameObject->SetFileName(fname);

	if (root.HasMember("Children"))
	{
		rapidjson::Value::Array childs = root["Children"].GetArray();
		for (unsigned int i = 0; i < childs.Size(); ++i)
		{
			GameObject* pChildObject = nullptr;
			if (childs[i].HasMember("File"))
			{
				pChildObject = LoadObject(childs[i]["File"].GetString());
			}
			else
			{
				pChildObject = LoadObject(childs[i].GetObject(),"");
			}
			pChildObject->mParentObject = pNewGameObject;
			pNewGameObject->mChildObjects.push_back(pChildObject);
		}
	}

	//New way to write components
	if (root.HasMember("Components"))
	{
		rapidjson::Value::Array comp = root["Components"].GetArray();
		for (unsigned int i = 0; i < comp.Size(); ++i)
		{
			std::string Comptype = comp[i]["Type"].GetString();
			pNewComponent = pNewGameObject->AddComponent(mComponentEnum[Comptype]);
			pNewComponent->Serialize(comp[i].GetObject());
			/*if (Comptype == "Particle")
			{
				ParticleSystem::GetInstance()->mParticleQueue.push_back((ParticleEmitter*)(pNewComponent));
			}*/
		}
	}

	GAMEOBJECT_MANAGER->mGameObjects.push_back(pNewGameObject);
	return pNewGameObject;
}

void GameObjectFactory::Close()
{
	for (auto a : mArchetypes)
	{
		delete a.second;
	}
	delete pObject;
}

GameObject* GameObjectFactory::GetArcheType(GameObjectType type)
{
	return LoadObject(mArchetypes[type]->GetObject(),""); //Fixme: Doesn't store filename
}