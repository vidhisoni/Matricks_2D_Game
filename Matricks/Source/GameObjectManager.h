/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once

class Component;
class GameObjectManager
{

	static GameObjectManager* pObject;
	GameObjectManager() {}

public:
	void Init();
	void Close();

	static GameObjectManager* CreateInstance();
	inline static GameObjectManager* GetInstance() { return pObject; }
	
	void Update();
	void DestoryQ();
	void DestroyAll();
	void RemoveInGameMenu();
	void ChangeInGameMenu();
	void EmptyMenuStack();
	void ClearMenuStack();

	void Pause();
	void UnPause();

	GameObject* GetPlayer();
	std::vector<GameObject *> GetValue();

private:
	void DeSerialize(std::string FileName);
	void LevelDeSerialize(std::ofstream* file, GameObject* obj);
public:
	std::vector<GameObject *> mGameObjects;
	std::map<unsigned int, Component *> mComponentsMap;
	bool buttonToggle = false;
	bool ObjectDetials = false;
	bool create = false;

	//TODO Remove(sai)
	bool test;
	void ObjectCreationHelper(GameObject* obj, float [], float []);
	void ChildObject(GameObject *obj, float[], float[]);
	glm::vec2 camStartpos;

public:
	 float bH[2] ;
	 float bV[2] ;
	 float camPos[2];
	 float levelbH[2] ;
	 float levelbV[2];
	 int previousSelection = 0;
	 Texture* PrevTex;
	 int i = 0;
	 bool changed = false;
	 int testt = 1;
	 std::stack<std::vector<GameObject*>*> mMenuStack;
	 bool res1, res2, res3;
	 glm::vec2 screenRes;

private:
	Texture* newTexture;

};
