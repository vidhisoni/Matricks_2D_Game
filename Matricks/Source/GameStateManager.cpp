/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
Modified by: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "SDL_keyboard.h"
#include "SDL_mouse.h"
#include <SDL.h>

///
#include <winerror.h> //for HRESULT
#include <comutil.h> //for _bstr_t (used in the string conversion)
#include "shlobj.h"
//

extern Engine* engine;

GameStateManager* GameStateManager::pObject = nullptr;

GameStateManager* GameStateManager::CreateInstance()
{
	pObject = new GameStateManager();
	return pObject;
}

void GameStateManager::Init()
{
	std::string fullPath = "Resources/Data/Levels/Levels.json";
	std::ifstream file(fullPath);
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	rapidjson::Document root;
	root.Parse(contents.c_str());
	Levels type;
	rapidjson::Value::Array levels = root["Levels"].GetArray();
	for (unsigned int i = 0; i < levels.Size(); ++i)
	{
		rapidjson::Value::Object obj = levels[i].GetObject();

		//Type
		type = (Levels)obj["Level"].GetInt();

		//File
		std::string FileName = obj["File"].GetString();
		std::string Path = "Resources/Data/Levels/";
		Path += FileName;
		std::ifstream file(Path);
		std::string levcontents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		rapidjson::Document* levdoc = new rapidjson::Document();
		levdoc->Parse(levcontents.c_str());
		mLevels[type] = levdoc;
	}

	//LevelReserializing Components
	mLevelSerialize.push_back(TRANSFORM);
	mLevelSerialize.push_back(SPRITE);
	mLevelSerialize.push_back(TEXT);
	mLevelSerialize.push_back(PLAYERSTATE);
	mLevelSerialize.push_back(BODY);
	mLevelSerialize.push_back(PROMPT);
	mLevelSerialize.push_back(SCRIPT);
	mLevelSerialize.push_back(COMP_TRIGGER);
	mLevelSerialize.push_back(AUDIO);
	mLevelSerialize.push_back(STORY);
	mLevelSerialize.push_back(AUDIO);
	mLevelSerialize.push_back(BUTTON);
	mChangeLevel = false;
	mNextLevel = false;

	//Load completed levels
	LPWSTR wszPath = NULL;
	HRESULT hr;
	hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &wszPath);

	if (SUCCEEDED(hr)) {
		_bstr_t bstrPath(wszPath);
		CompletedLevelsFile = std::string((char*)bstrPath) + "\\Matricks\\";
		CreateDirectory(CompletedLevelsFile.c_str(), NULL);
		CompletedLevelsFile += "CompletedLevels.json";
	}

	std::ifstream cfile(CompletedLevelsFile);
	if (!cfile.good())
	{
		std::ofstream savefile;
		savefile.open(CompletedLevelsFile);
		savefile << "{\"Levels\":[]}";
		savefile.close();
	}
	cfile.open(CompletedLevelsFile);
	std::string ccontents((std::istreambuf_iterator<char>(cfile)), std::istreambuf_iterator<char>());

	root.Parse(ccontents.c_str());

	rapidjson::Value::Array clevels = root["Levels"].GetArray();

	unsigned int size = clevels.Size();
	for (unsigned int i = 0; i < clevels.Size(); ++i)
	{
		mCompletedLevels.push_back(clevels[i].GetUint());
	}
}

void GameStateManager::Update()
{
	if (engine->IsPaused())
		return;
	if ((INPUT_MANAGER->IsTriggered(SDL_SCANCODE_SPACE) || INPUT_MANAGER->IsMouseClicked(SDL_BUTTON_RIGHT)) && mCurrentLevel < LEVEL1 && mCurrentLevel > MAINMENU)
	{
		ChangeLevel(LEVEL1);
	}
	if ((INPUT_MANAGER->IsTriggered(SDL_SCANCODE_SPACE) || INPUT_MANAGER->IsMouseClicked(SDL_BUTTON_RIGHT)) && mCurrentLevel > LEVEL14 && mCurrentLevel < CREDITS)
	{
		ChangeLevel(CREDITS);
	}
	if ((INPUT_MANAGER->IsTriggered(SDL_SCANCODE_SPACE) || INPUT_MANAGER->IsMouseClicked(SDL_BUTTON_RIGHT)) && mCurrentLevel == CREDITS)
	{
		ChangeLevel(MAINMENU);
	}
	if (mChangeLevel || (RENDER_MANAGER->GetFadeValue() < 0.2f && mNextLevel))
	{
		LoadLevel(mCurrentLevel);	
		RENDER_MANAGER->FadeIn();
		mChangeLevel = false;
		mNextLevel = false;
	}
}

void GameStateManager::Close()
{
	for (auto obj : mLevels)
		delete obj.second;

	delete pObject;
}

void GameStateManager::LoadLevel(Levels pLevelName)
{
	AUDIO_MANAGER->StopSongs();
	AUDIO_MANAGER->StopPrompt();
	AUDIO_MANAGER->StopSFXs();
	GAMEOBJECT_MANAGER->ClearMenuStack();
	GAMEOBJECT_MANAGER->DestroyAll();
	EVENT_MANAGER->ClearTimedEvents();

	AudioManager::GetInstance()->currentPromptPath.clear();
	AudioManager::GetInstance()->currentSFXPath.clear();
	AudioManager::GetInstance()->currentSongPath.clear();


	if (pLevelName > MAINMENU)
		engine->HideCursor();
	else
		engine->ShowCursor();

	startScript = true;
	rapidjson::Value::Object root = mLevels[pLevelName]->GetObject();
	mCurrentLevel = pLevelName;

	RENDER_MANAGER->mCamera.Reset();
	if (root.HasMember("Camera"))
	{
		RENDER_MANAGER->mCamera.Serialize(root["Camera"].GetObject());
	}
	rapidjson::Value::Array objects = root["Objects"].GetArray();

	for (unsigned int i = 0; i < objects.Size(); ++i)
	{
		GameObject *pGameObject = nullptr;
		rapidjson::Value::Object obj = objects[i].GetObject();

		if (obj.HasMember("File"))
		{
			pGameObject = GAMEOBJECT_FACTORY->LoadObject(obj["File"].GetString());
		}
		if (obj.HasMember("Children"))
		{
			rapidjson::Value::Array childs = obj["Children"].GetArray();
			for (unsigned int i = 0; i < childs.Size(); ++i)
			{
				rapidjson::Value::Object child = childs[i].GetObject();
				GameObject* pChild = nullptr;
				if (child.HasMember("Type"))
				{
					pChild = pGameObject->ChildFind(GAMEOBJECT_FACTORY->GetGameObjectEnum(child["Type"].GetString()));
				}
				if (child.HasMember("Index"))
				{
					pChild = pGameObject->mChildObjects[child["Index"].GetInt()];
				}
				LevelReSerialize(child, pChild);
			}
		}
		LevelReSerialize(obj, pGameObject);
	}
	PHYSICS_MANAGER->nsq.m_aabbs.clear();
	delete PHYSICS_MANAGER->DynTree.m_root;
	PHYSICS_MANAGER->DynTree.m_root = nullptr;
	//PHYSICS_MANAGER->DynTree.m_pairs.clear();
	for (auto c : GAMEOBJECT_MANAGER->mGameObjects)
	{
		Body*pBody = static_cast<Body*>(c->GetComponent(BODY));
		if (pBody == nullptr)
			continue;
		PHYSICS_MANAGER->nsq.Add(pBody);
		PHYSICS_MANAGER->DynTree.Add(pBody);
	}

	RENDER_MANAGER->mCamera.Init();
}

void GameStateManager::LoadInGameMenu(Levels menu)
{
	GAMEOBJECT_MANAGER->EmptyMenuStack();
	SerializeInGameMenu(menu);
}

void GameStateManager::AddCompletedLevel(Levels level)
{
	if (level < LEVEL1 || level > LEVEL14)
		return;

	//std::string mFilename = "Resources/Data/Levels/CompletedLevels.json";
	

	std::ifstream file(CompletedLevelsFile);
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	rapidjson::Document root;
	root.Parse(contents.c_str());

	rapidjson::Value::Array levels = root["Levels"].GetArray();

	if(std::find(mCompletedLevels.begin(),mCompletedLevels.end(),level) == mCompletedLevels.end())
		mCompletedLevels.push_back(level);

	if (mCompletedLevels.size() > levels.Size())
	{
		std::ofstream savefile;
		savefile.open(CompletedLevelsFile);
		savefile << "{\"Levels\":[";
		for (unsigned int i = 0; i < mCompletedLevels.size() - 1; ++i)
		{
			savefile << mCompletedLevels[i] << ",";
		}
		savefile << mCompletedLevels[mCompletedLevels.size() - 1] << "]}";
		savefile.close();
	}
}

void GameStateManager::SerializeInGameMenu(Levels menu)
{
	rapidjson::Value::Object root = mLevels[menu]->GetObject();
	rapidjson::Value::Array objects = root["Objects"].GetArray();

	std::vector<GameObject*> *list = new std::vector<GameObject*>();

	for (unsigned int i = 0; i < objects.Size(); ++i)
	{
		GameObject *pGameObject = nullptr;
		rapidjson::Value::Object obj = objects[i].GetObject();

		if (obj.HasMember("File"))
		{
			pGameObject = GAMEOBJECT_FACTORY->LoadObject(obj["File"].GetString());
		}
		LevelReSerialize(obj, pGameObject);
		list->push_back(pGameObject);
	}
	GAMEOBJECT_MANAGER->mMenuStack.push(list);
}
void GameStateManager::ChangeInGameMenu(Levels menu)
{	
	GAMEOBJECT_MANAGER->ChangeInGameMenu();
	GAMESTATE_MANAGER->SerializeInGameMenu(menu);
}

void GameStateManager::LevelReSerialize(rapidjson::Value::Object obj, GameObject* pGameObject)
{
	Body *pBody = nullptr;
	for (auto c : mLevelSerialize)
	{
		switch (c)
		{
		case TRANSFORM:
			if (obj.HasMember("Transform")) {
				static_cast<Transform*>(pGameObject->GetComponent(c))->Serialize(obj["Transform"].GetObject());
				pBody = static_cast<Body*>(pGameObject->GetComponent(BODY));
				if (pBody != nullptr)
					pBody->Initialize();
			}
			break;
		case SPRITE:
			if (obj.HasMember("Sprite"))
				static_cast<Sprite*>(pGameObject->GetComponent(c))->Serialize(obj["Sprite"].GetObject());
			break;
		case TEXT:
			if (obj.HasMember("Text"))
				static_cast<Text*>(pGameObject->GetComponent(c))->Serialize(obj["Text"].GetObject());
			break;
		case PLAYERSTATE:
			if (obj.HasMember("PlayerState"))
				static_cast<PlayerState*>(pGameObject->GetComponent(c))->Serialize(obj["PlayerState"].GetObject());
			break;
		case BODY:
			if (obj.HasMember("Body"))
			{
				static_cast<Body*>(pGameObject->GetComponent(c))->Serialize(obj["Body"].GetObject());
				Body* pbody = static_cast<Body*>(pGameObject->GetComponent(c));
				pbody->Initialize();
			}
			break;
		case PROMPT:
			if (obj.HasMember("Prompt"))
				static_cast<Prompt*>(pGameObject->GetComponent(c))->Serialize(obj["Prompt"].GetObject());
			break;
		case COMP_TRIGGER:
			if (obj.HasMember("Trigger"))
				static_cast<Prompt_Trigger*>(pGameObject->GetComponent(c))->Serialize(obj["Trigger"].GetObject());
			break;
		case SCRIPT:
			if (obj.HasMember("Script"))
				static_cast<Script*>(pGameObject->GetComponent(c))->Serialize(obj["Script"].GetObject());
			break;
		case STORY:
			if(obj.HasMember("Story"))
				static_cast<Story*>(pGameObject->GetComponent(c))->Serialize(obj["Story"].GetObject());
			break;
		case AUDIO:
			if (obj.HasMember("Audio"))
				static_cast<Audio*>(pGameObject->GetComponent(c))->Serialize(obj["Audio"].GetObject());
			break;
		case BUTTON:
			if (obj.HasMember("Button"))
				static_cast<Button*>(pGameObject->GetComponent(c))->Serialize(obj["Button"].GetObject());
			break;
		}
	}
}

void GameStateManager::NextLevel()
{
	if (!mNextLevel)
	{
		if (mCurrentLevel == CREDITS)
			ChangeLevel(MAINMENU);
		else
		{
			AddCompletedLevel(mCurrentLevel);
			mCurrentLevel = (Levels)(mCurrentLevel + 1);
			mNextLevel = true;
			RENDER_MANAGER->FadeOut();
		}
	}
}