/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"


#define POW(x) (unsigned int)pow(2.0,x)

EventManager* EventManager::pObject = nullptr;


Event::Event(EventType Type, GameObject* Object,Contact* contact)
{
	pObject = Object;
	mType = Type;
	mTimer = 0.0f;
	mContact = contact;
}

void EventManager::Init()
{
	//Events enum map
	mEventEnumMap["CameraMove"] = CAMERAMOVE;
	mEventEnumMap["CameraZoomIn"] = CAMERAZOOMIN;
	mEventEnumMap["NumberBoxCollision"] = NUMBERBOXCOLLISION;
	mEventEnumMap["ChangeNumber"] = CHANGENUMBER;
	mEventEnumMap["TrapCollision"] = TRAPCOLLISION;
	mEventEnumMap["CameraZoomOut"] = CAMERAZOOMOUT;
	mEventEnumMap["OnGroundCollision"] = ONGROUNDCOLLISION;
	mEventEnumMap["IntermediateDoor"] = INTERMEDIATEDOOR;
	mEventEnumMap["LevelComplete"] = LEVELCOMPLETE;
	mEventEnumMap["DivideBox"] = DIVIDEBOX;
	mEventEnumMap["AIMovement"] = AIMOVEMENT;
	mEventEnumMap["AlarmEvent"] = ALARMEVENT;
	mEventEnumMap["Test"] = TEST;
	mEventEnumMap["ChargeUsed"] = CHARGEUSED;
	mEventEnumMap["DeleteDoor"] = DELETEDOOR;
	mEventEnumMap["PromptTrigger"] = PROMPT_TRIGGER;
	mEventEnumMap["ChangeLevel"] = CHANGELEVEL;
	mEventEnumMap["NextFrame"] = NEXTFRAME;
	mEventEnumMap["OpenHUDDoor"] = OPENHUDDOOR;
	mEventEnumMap["OutOfBound"] = OUTOFBOUND;
	mEventEnumMap["Box_Ai"] = BOX_AI;
	mEventEnumMap["Box_Ai1"] = BOX_AI1;
	mEventEnumMap["Box_Ai2"] = BOX_AI2;
	mEventEnumMap["Num_Ai"] = NUM_AI;


	//GameObjects Events Map
	mEventMap[POW(PLAYER) | POW(NUMBOX)] = NUMBERBOXCOLLISION;//asise krna hai XD
	mEventMap[POW(PLAYER) | POW(BOXTRIGGER)] = NUMBERBOXTRIGGER;//asise krna hai XD
	mEventMap[POW(PLAYER) | POW(TRAP)] = TRAPCOLLISION;
	mEventMap[POW(PLAYER) | POW(PLATFORM)] = ONGROUNDCOLLISION;
	mEventMap[POW(PLAYER) | POW(IDOOR)] = INTERMEDIATEDOOR;
	mEventMap[POW(PLAYER) | POW(FINALDOOR)] = LEVELCOMPLETE;
	mEventMap[POW(NUMBOX) | POW(PLATFORM)] = DIVIDEBOX;
	mEventMap[POW(ENEMY) | POW(PLATFORM)] = AIMOVEMENT;
	mEventMap[POW(TESTAIBOT) | POW(PLATFORM)] = AIMOVEMENT;
	mEventMap[POW(PLAYER) | POW(ALARM)] = ALARMEVENT;
	mEventMap[POW(PLAYER) | POW(ENEMY)] = L_AICOLLISION;
	//mEventMap[POW(PLAYER) | POW(ENEMY)] = ENEMYHIT;
	mEventMap[POW(PLAYER) | POW(TESTAIBOT)] = ENEMYHIT;
	mEventMap[POW(TESTAIBOT) | POW(TRIGGEROBJ)] = TRIGGER;
	mEventMap[POW(PLAYER) | POW(P_TRIGGER)] = PROMPT_TRIGGER;
	mEventMap[POW(PLAYER) | POW(CHARGE)] = CHARGEPICKUP;
	mEventMap[POW(PLAYER) | POW(OUTBOUND)] = OUTOFBOUND;
	mEventMap[POW(TESTAIBOT) | POW(FINALDOOR)] = BOX_AI;
	mEventMap[POW(TESTAIBOT) | POW(STARTDOOR)] = BOX_AI;
	mEventMap[POW(TESTAIBOT) | POW(BOX)] = BOX_AI2;
	mEventMap[POW(TESTAIBOT) | POW(NUMBOX)] = NUM_AI;

}

void EventManager::Close()
{
	delete pObject;
}

EventManager * EventManager::CreateInstance()
{
	pObject = new EventManager();
	return pObject;
}

void EventManager::BroadCastEvent(Event * pEvent)
{
	for (unsigned int i = 0; i < GameObjectManager::GetInstance()->mGameObjects.size(); ++i)
		GameObjectManager::GetInstance()->mGameObjects[i]->HandleEvent(pEvent);
}

void EventManager::BroadCastEventToSubscribers(Event * pEvent)
{
	auto &listOfSubscribers = mSubscribe[pEvent->mType];

	for (auto pGO : listOfSubscribers)
	{
		pGO->HandleEvent(pEvent);
	}
}

void EventManager::AddTimedEvent(Event * pEvent)
{
	mTimedEvents.push_back(pEvent);
}

void EventManager::Update(float FrameTime)
{

	std::list<Event *>::iterator it;

	it = mTimedEvents.begin();

	while (it != mTimedEvents.end())
	{
		Event *pEvent = *it;
		pEvent->mTimer -= FrameTime / 1000.0f;
		if (pEvent->mTimer < 0.0f)
		{
			BroadCastEventToSubscribers(pEvent);

			delete pEvent;
			it = mTimedEvents.erase(it);
		}
		else
			++it;
	}
}

void EventManager::Subscribe(EventType Type, GameObject * pGameObjectNewSubscriber)
{
	//std::list<GameObject *> &listOfSubscribes = mSubscribe[Type];
	auto &listOfSubscribers = mSubscribe[Type];

	for (auto pGO : listOfSubscribers)
		if (pGO == pGameObjectNewSubscriber)
			return;

	listOfSubscribers.push_back(pGameObjectNewSubscriber);
}

void EventManager::UnSubscribe(EventType Type, GameObject * pGameObjectUnsubscriber)
{
	auto &listOfSubscribers = mSubscribe[Type];
	auto it = std::find(listOfSubscribers.begin(), listOfSubscribers.end(), pGameObjectUnsubscriber);

	if (it == listOfSubscribers.end())
		return;

	listOfSubscribers.erase(it);
}

void EventManager::ClearTimedEvents()
{
	for (auto e : mTimedEvents)
		delete e;

	mTimedEvents.clear();
}

