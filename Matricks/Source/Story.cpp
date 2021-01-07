/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"

Story::Story() :  Component(STORY)
{
	mDone = false;
	isLastFrame = false;
	mStart = false;
	isSound = false;
}

Story::~Story()
{

}

void Story::Update()
{
	if (mpOwner->isPaused)
		return;
	Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));		

	if (!mDone && mStart)
	{
		if (isSound)
		{
			Audio* audio = static_cast<Audio*>(mpOwner->GetComponent(AUDIO));
			//	std::cout << "Sound Played at 1 " << "FRAME NUMBER : " << mFrameNum << std::endl;
			audio->Play(0);
		}

			float distance = ((ENGINE_HEIGHT / 2.0) - pTr->mPosition.y) * 10.0f * FRAMERATE_CONTROLLER->GetFrameTime() / 1000.0f;
			pTr->mPosition.y += distance;
			if (fabs(pTr->mPosition.y - (ENGINE_HEIGHT / 2.0)) < 0.1f)
			{
				mDone = true;
				Event *pEvent = new Event(NEXTFRAME);
				pEvent->mTimer = mTime;
				EVENT_MANAGER->AddTimedEvent(pEvent);
			}
	}
}

void Story::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("LastFrame"))
	{
		isLastFrame = true;
	}
	if (obj.HasMember("Frame"))
	{
		mFrameNum = obj["Frame"].GetInt();
		if (mFrameNum == 0)
			mStart = true;
	}
	if (obj.HasMember("isSound"))
	{
		isSound = obj["isSound"].GetBool();		
	}
	if (obj.HasMember("Time"))
	{
		mTime = obj["Time"].GetFloat();
	}
}

void Story::HandleEvent(Event* pEvent)
{
	if (pEvent->mType == NEXTFRAME)
	{
		mFrameNum--;
		if (mFrameNum == 0)
			mStart = true;
		if (isLastFrame && mFrameNum == -1) 
			GAMESTATE_MANAGER->NextLevel();
	}
}

void Story::Clear()
{
	mDone = false;
	isLastFrame = false;
	mStart = false;
}

Component* Story::Create()
{
	return new Story();
}

