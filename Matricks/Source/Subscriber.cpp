/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"

Subsciber::Subsciber() : Component(SUBSCRIPTION)
{

}


void Subsciber::Update()
{
	
}

void Subsciber::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("SubscribeTo")) {
		rapidjson::Value::Array mEvents = obj["SubscribeTo"].GetArray();
		for (unsigned int i = 0; i < mEvents.Size(); ++i)
		{
			std::string type = mEvents[i].GetString();
			EventManager* pEventMgr = EventManager::GetInstance();
			pEventMgr->Subscribe(pEventMgr->GetEventEnum(type), mpOwner);
			mEventsSubscribed.push_back(type);
		}
	}
}

void Subsciber::DeSerialize(std::ofstream* file)
{
	*file << "{\n\"Type\":\"Subscription\",\n";
	*file << "\"SubscribeTo\":[";
	for (auto e : mEventsSubscribed)
	{
		*file <<"\"" << e << "\",";
	}
	long pos = (long)file->tellp();
	file->seekp(pos - 1);
	*file << "]\n},\n";
}

void Subsciber::Clear()
{
	EventManager* pEventMgr = EventManager::GetInstance();
	for (unsigned int i = 0; i < mEventsSubscribed.size(); ++i)
	{
		pEventMgr->UnSubscribe(pEventMgr->GetEventEnum(mEventsSubscribed[i]), mpOwner);
	}
	mEventsSubscribed.clear();
}

Component* Subsciber::Create()
{
	return new Subsciber();
}

void Subsciber::HandleEvent(Event* pEvent)
{

}

void Subsciber::SubscribeToEvent()
{

}

