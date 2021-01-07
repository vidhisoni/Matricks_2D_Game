/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

//To add a new component:
//1-Add the enum to the list below
//2-Create 'new' and initialize the map in GameObjectManager Init()
//3-Add the component to the enum map in GameObjectFactory Init()
enum ComponentType
{
	TRANSFORM,
	SPRITE,
	CONTROLLER,
	BODY,
	STATE,
	ANIMATOR,
	AUDIO,
	TEXT,
	SUBSCRIPTION,
	PLAYERSTATE,
	PARALLAX,
	PARTICLE,
	LIGHT,
	CLOUD,
	BUTTON,
	SCRIPT,
	LINERAI,
	ALARMCOMPONENT,
	AIENEMY,
	KEY,
	TESTAI,
	TRIGGERCOMPONENT,
	COLLIDER,
	PROMPT,
	COMP_TRIGGER,
	STORY,
	MENUCONTROL,
	LEVEL,
	COMP_NUM,
};

class GameObject;
class Event;

class Component
{
public:
	Component(ComponentType Type);
	virtual ~Component() { }

	virtual void Update() = 0;
	inline ComponentType GetType() { return mType; }

	virtual void Serialize(rapidjson::Value::Object obj) {}
	virtual void DeSerialize(std::ofstream* file) {}

	virtual Component* Create() = 0;
	virtual void Clear() = 0;
	virtual void HandleEvent(Event* pEvent) {}

public:

	GameObject *mpOwner;

private:

	ComponentType mType;

};
