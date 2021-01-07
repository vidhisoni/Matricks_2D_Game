/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class Texture;

class Animator : public Component
{
public:
	Animator();

	void Update();

	void Serialize(rapidjson::Value::Object obj);
	
	Component* Create();
	void Clear();
	void SetState(std::string state);
	inline void SetState(std::string state, float time) { mCurrState = state; mTimeFactor = time; }
	inline void SetPrevState(std::string state) { mPrevState = state; }
	inline std::string GetState() { return mCurrState; }
	void HandleEvent(Event* pEvent);
	void PlayAnimation(std::string animation, EventType event = NOEVENT, float timefactor = 1.0f);

private:
	std::map<std::string, std::vector<Texture*>> mAnimations;
	float mCurrTime;
	int mCurrFrame;
	std::string mCurrState, mPrevState;
	float mTimeFactor, mPrevTimeFactor;
	bool mStateChanged;
	bool mAnimationPlay;
	EventType mPostEvent;
};