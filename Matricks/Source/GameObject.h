/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once
//To add a new Game Object:
//1-Add the enum to the list below
//2-Add the enum in the map in GameObjectFactory Init()
enum GameObjectType
{
	NOOBJECT = -1,
	PLAYER = 1,
	PLATFORM,
	ENEMY,
	BACKGROUND,
	NUMBOX,
	IDOOR,
	BUTTONOBJ,
	TRAP,
	FINALDOOR,
	INVISIPLATFORM,
	ALARM,
	BOUNCY,
	UnIDoor,
	AI_ENEMY,
	TESTAIBOT,
	TRIGGEROBJ,
	STARTDOOR,
	PROMPTER,
	P_TRIGGER,
	BOX,
	CHARGE,
	NUMCLOUD,
	LIFT,
	VALUE,
	BOXTRIGGER,
	PORTAL,
	OUTBOUND,
	OBJ_NUM
};

class GameObject
{
public:
	GameObject();

	void Update();
	Component *AddComponent(unsigned int Type);
	Component *GetComponent(unsigned int Type);
	void HandleEvent(Event *pEvent);
	void Clear();

	inline void SetType(GameObjectType Type) { mObjectType = Type; mHash = pow(2.0, Type); }
	inline GameObjectType GetType() { return mObjectType; }
	inline void SetFileName(std::string file) { mFileName = file; }
	inline std::string GetFileName() { return mFileName; }
	GameObject* ChildFind(GameObjectType find);
	inline void Destroy() { mDestroyed = true; }
	inline bool isDestroyed() { return mDestroyed; }

	void DeleteChild(GameObject* child);
	
	//ChildObjects and ParentObject
	std::vector<GameObject*> mChildObjects;
	GameObject* mParentObject;
	unsigned int  mHash;
	bool emitParticle;
	bool isPaused;
	bool mActive;

private:
	GameObjectType mObjectType;
	std::vector<Component *> mComponents;
	std::string mFileName;
	bool mDestroyed;

};

