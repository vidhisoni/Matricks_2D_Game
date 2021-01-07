/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"

State::State() : Component(STATE)
{

}

void State::Update()
{
	Animator* pAnim = static_cast<Animator*>(mpOwner->GetComponent(ANIMATOR));
	switch (mCurrentState)
	{
	case ACTIVE:
		pAnim->SetState("active");
		break;
	case CLOSED:
		pAnim->SetState("closed");
		break;
	case OPENING:
		pAnim->PlayAnimation("opening");
		break;
	case OPENED:
		pAnim->SetState("opened");
		break;
	}
}

void State::Serialize(rapidjson::Value::Object obj)
{
	//std::wstring wt = obj[L"Default"]->AsString();
	//mCurrentState = std::string(wt.begin(), wt.end());
	//
	//JSONArray states = obj[L"States"]->AsArray();
	//for (unsigned int i = 0; i < states.size(); ++i)
	//{
	//	std::wstring wt = states[i]->AsString();
	//	std::string sst = std::string(wt.begin(), wt.end());
	//	if (mCurrentState == sst)
	//		mStates[sst] = true;
	//	else
	//		mStates[sst] = false;
	//}
}

void State::DeSerialize(std::ofstream* file)
{
	*file << "{\n\"Type\":\"State\"},\n";
}

Component* State::Create()
{
	return new State();
}

void State::Clear()
{
}

