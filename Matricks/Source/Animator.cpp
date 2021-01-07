/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"

Animator::Animator() : Component(ANIMATOR), mCurrTime(0.0f), mCurrFrame(0), mTimeFactor(1.0f), mStateChanged(false)
{
}

void Animator::Update()
{
	if (mpOwner->isPaused)
		return;
	//if (mpOwner->GetType() == FINALDOOR)
	//	return;
	Sprite *pSp = static_cast<Sprite*>(mpOwner->GetComponent(SPRITE));
	State *pSt = static_cast<State*>(mpOwner->GetComponent(STATE));
	mCurrTime += FRAMERATE_CONTROLLER->GetFrameTime() / 1000.0f;
	if (mCurrTime > (mTimeFactor / mAnimations[mCurrState].size()) || mStateChanged)
	{
		if (mCurrFrame < (int)mAnimations[mCurrState].size())
			pSp->SetTexture(mAnimations[mCurrState][mCurrFrame++]);
		if (mCurrFrame >= (int)mAnimations[mCurrState].size())
		{
			if (mAnimationPlay)
			{
				mAnimationPlay = false;
				mCurrState = mPrevState;
				mTimeFactor = mPrevTimeFactor;
				if (mPostEvent != NOEVENT)
				{
					/*Event pEvent(mPostEvent);
					EVENT_MANAGER->BroadCastEventToSubscribers(&pEvent);*/
					Event* pEvent = new Event(mPostEvent);
					pEvent->mTimer = 1.5;
					EventManager::GetInstance()->AddTimedEvent(pEvent);
					mPostEvent = NOEVENT;
				}
			}
			mCurrFrame = 0;
		}
		mCurrTime = 0.0f;
	}
}

void Animator::Serialize(rapidjson::Value::Object obj)
{
	rapidjson::Value::Array Anims = obj["SpriteSheet"].GetArray();
	for (unsigned int i = 0; i < Anims.Size(); ++i)
	{
		rapidjson::Value::Object sa = Anims[i].GetObject();

		std::string Name = sa["Type"].GetString();
		mAnimations[Name] = ResourceManager::GetInstance()->GetAnimation(sa["Sheet"].GetString());
	}
}

Component* Animator::Create()
{
	return new Animator();
}

void Animator::Clear()
{
	mAnimations.clear();
	mCurrTime = 0.0f;
	mCurrFrame = 0;
	mCurrState = mPrevState = "";
	mTimeFactor = mPrevTimeFactor = 0.0f;
	mStateChanged = mAnimationPlay = false;
	mPostEvent = NOEVENT;
}

void Animator::SetState(std::string state)
{
	if (mCurrState != state && !mAnimationPlay)
	{
		mCurrState = state;
		mStateChanged = true;
		mCurrFrame = 0;
		mCurrTime = 0.0f;
	}
	else
	{
		mStateChanged = false;
	}
}

void Animator::HandleEvent(Event * pEvent)
{
	if (pEvent->mType == TEST)
	{
		RENDER_MANAGER->SetAmbientLight(0.7f);
	}
}

void Animator::PlayAnimation(std::string animation, EventType event, float timefactor)
{
	if (animation != mCurrState || animation == "jumpleft" || animation == "jumpright")
	{
		mAnimationPlay = true;
		mPrevState = mCurrState;
		mPrevTimeFactor = mTimeFactor;
		mPostEvent = event;
		mTimeFactor = timefactor;
		mCurrState = animation;
		mStateChanged = true;
		mCurrFrame = 0;
		mCurrTime = 0.0f;
	}
	else
	{
		mStateChanged = false;
	}
}
