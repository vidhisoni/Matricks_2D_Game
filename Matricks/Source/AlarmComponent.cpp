/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Sairaj Padghe
Modified By: Sai Sindhu Jannali
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "AlarmComponent.h"
#include "EventManager.h"

AlarmComponent::AlarmComponent():Component(ALARMCOMPONENT)
{
	inOUt = false;
	playAudio = false;
}

void AlarmComponent::Update()
{
	if (inOUt)
	{
		RENDER_MANAGER->SetAmbientLight(0.15f);
	}
	else
	{
		RENDER_MANAGER->SetAmbientLight(0.7f);
	}
	if(inOUt)
		inOUt = false;
}

void AlarmComponent::DeSerialize(std::ofstream* file)
{

}

void AlarmComponent::Clear()
{

}

Component* AlarmComponent::Create()
{
	return new AlarmComponent();
}

void AlarmComponent::HandleEvent(Event* pEvent)
{
	if (pEvent->mType == ALARMEVENT) {
		inOUt = true;
		playAudio = true;
		((Audio*)mpOwner->GetComponent(AUDIO))->Play(0);
		//std::cout << "Audio playeed" << std::endl;
		//AudioManager::GetInstance()->PauseMusic();
	//	((Audio*)mpOwner->GetComponent(AUDIO))->Play(1);
		Event Charge(PLAYERDETECT, nullptr);
		EventManager::GetInstance()->BroadCastEventToSubscribers(&Charge);
	}
}

