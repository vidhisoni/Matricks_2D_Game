/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once
class GameObject;
//To add a new event:
//1-Add the enum to the list below
//2-Add the event to the enum map in EventManager Init()

enum EventType
{
	NOEVENT = -1,
	CAMERAMOVE = 1,
	CAMERAZOOMIN,
	NUMBERBOXCOLLISION,
	CHANGENUMBER,
	TRAPCOLLISION,
	CAMERAZOOMOUT,
	ONGROUNDCOLLISION,
	INTERMEDIATEDOOR,
	LEVELCOMPLETE,
	DIVIDEBOX,
	AIMOVEMENT,
	ALARMEVENT,
	TEST,
	CHARGEUSED,
	DELETEDOOR,
	L_AICOLLISION,
	AICOLLISION,
	ENEMYHIT,
	TRIGGER,
	PLAYERDETECT,
	CHARGEPICKUP,
	CHANGELEVEL,
	PROMPT_TRIGGER,
	NEXTFRAME,
	NUMBERBOXTRIGGER,
	OPENHUDDOOR,
	BOX_AI,
	BOX_AI1,
	BOX_AI2,
	NUM_AI,
	OUTOFBOUND
};

class Event
{
public:
	Event(EventType Type, GameObject* Object= nullptr,Contact* contact=nullptr);
	virtual ~Event() {}

public:
	float mTimer;
	GameObject* pObject;
	EventType mType;
	Contact* mContact;
};

class EventManager
{
	static EventManager* pObject;
	EventManager() {}

public:
	
	void Init();
	void Close();

	static EventManager* CreateInstance();
	inline static EventManager* GetInstance() { return pObject; }

	void BroadCastEvent(Event *pEvent);
	void BroadCastEventToSubscribers(Event *pEvent);
	void AddTimedEvent(Event *pEvent);
	void Update(float FrameTime);
	void Subscribe(EventType Type, GameObject *pGameObject);
	void UnSubscribe(EventType Type, GameObject *pGameObjectUnsubscriber);
	void ClearTimedEvents();

	inline EventType GetEventEnum(std::string event) { return mEventEnumMap[event]; }

public:
	std::map<unsigned int, EventType> mEventMap;

private:
	std::list<Event *>mTimedEvents;
	std::map<std::string, EventType> mEventEnumMap;
	std::unordered_map < EventType, std::list<GameObject *>> mSubscribe;
};