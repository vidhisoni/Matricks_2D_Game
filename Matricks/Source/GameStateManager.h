/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
Modified by: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once

enum ComponentType;

enum Levels
{
	LEVELTEMPLATE = -14,
	STORYPAUSE,
	LEVELLIST,
	QUITCONFIRM,
	HOWTOPLAY,
	SETTINGS,
	PAUSEMENU,
	MAINMENU,
	STORYLINE1,
	STORYLINE2,
	STORYLINE3,
	STORYLINE4,
	STORYLINE5,
	STORYLINE6,
	STORYLINE7,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4,
	LEVEL5,
	LEVEL6,
	LEVEL7,
	LEVEL8,
	LEVEL9,
	LEVEL10,
	LEVEL11,
	LEVEL12,
	LEVEL13,
	LEVEL14,
	ENDSTORY1,
	ENDSTORY2,
	ENDSTORY3,
	ENDSTORY4,
	CREDITS,
};

class GameStateManager
{
	static GameStateManager* pObject;
	GameStateManager(){ }
public:

	static GameStateManager* CreateInstance();
	inline static GameStateManager* GetInstance() { return pObject; }
	void Init();
	void Update();
	void Close();
	void LoadLevel(Levels pLevelName);
	inline Levels GetCurrentLevel() { return mCurrentLevel; }
	void LoadInGameMenu(Levels menu);
	void ChangeInGameMenu(Levels menu);
	void NextLevel();
	void AddCompletedLevel(Levels level);
	inline void ChangeLevel(Levels level) { 
		if (!mChangeLevel) 
		{ 
			mCurrentLevel = level; 
			mChangeLevel = true;
			RENDER_MANAGER->FadeOut();
		}
	}
	
public:
	std::vector<ComponentType> mLevelSerialize;
	bool startScript = false;
	std::vector<unsigned int> mCompletedLevels;
private:
	void LevelReSerialize(rapidjson::Value::Object obj, GameObject* pGameObject);
	void SerializeInGameMenu(Levels menu);
private:
	std::map<Levels, rapidjson::Document*> mLevels;
	Levels mCurrentLevel;
	bool mChangeLevel, mNextLevel;
	std::string CompletedLevelsFile;
};



