/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"

#include"SDL_keyboard.h"
Cloud::Cloud() : Component(CLOUD)
{
	mEnable = false;
	mUsed = false;
}

Cloud::~Cloud()
{

}

void Cloud::Update()
{
	Transform* pTr = static_cast<Transform*>(mpOwner->mParentObject->GetComponent(TRANSFORM));
	pTrans->mPosition = mOffset * pTr->mScale + pTr->mPosition;
	if (mEnable)
	{
		if (pTrans->mScale.x < mScale.x)
		{
			pTrans->mScale += 4.0f * mScale.x * FRAMERATE_CONTROLLER->GetFrameTime() / 1000.0f;
		}
		else
			mEnable = false;
	}
	mUsed = false;
}

void Cloud::Serialize(rapidjson::Value::Object obj)
{
	pTrans = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	if (obj.HasMember("Offset"))
	{
		rapidjson::Value::Array offset = obj["Offset"].GetArray();
		mOffset.x = offset[0].GetFloat();
		mOffset.y = offset[1].GetFloat();
	}
}

void Cloud::DeSerialize(std::ofstream* file)
{
	*file << "{\n\"Type\":\"Cloud\",\n";
	*file << "\"Offset\":[" << mOffset.x << "," << mOffset.y << "]\n},\n";
}

void Cloud::HandleEvent(Event* pEvent)
{
	if (pEvent->mType == CHANGENUMBER && !mUsed)
	{
		Text* pBoxTx = static_cast<Text*>(pEvent->pObject->mParentObject->GetComponent(TEXT));
		if (pBoxTx->IsActive())
		{
			PlayerState *pState = static_cast<PlayerState*>(mpOwner->mParentObject->GetComponent(PLAYERSTATE));
			
			Text* pTx1 = static_cast<Text*>(pEvent->pObject->mParentObject->GetComponent(TEXT));
			if (!pTx1 || pTx1->GetText() == "")
				return;
			int ObjectValue = std::stoi(pTx1->GetText());
			Text* pTx = static_cast<Text*>(mpOwner->GetComponent(TEXT));
			if (!pTx)
				return;
			int value;
			bool changed = false;
			//Pickup
			if (pTx->GetText() == "" && (InputManager::GetInstance()->IsButtonTriggered(SDL_CONTROLLER_BUTTON_Y) || InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_E)))
			{
				Audio* audio = static_cast<Audio*>(pEvent->pObject->mParentObject->GetComponent(AUDIO));
				audio->Play(0);

				changed = true;
				value = ObjectValue;
				pTx->SetText(std::to_string(value));
				mEnable = true;
				//pTrans->mScale = mScale;
			}
			else if (pTx->GetText() != "")
			{
				value = std::stoi(pTx->GetText());
				if (pEvent->pObject->mParentObject->GetType() == NUMBOX)
				{
					Audio* audio = static_cast<Audio*>(pEvent->pObject->mParentObject->GetComponent(AUDIO));
					//Pickup
					if (InputManager::GetInstance()->IsButtonTriggered(SDL_CONTROLLER_BUTTON_Y) || InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_E))
					{
						audio->Play(0);
						value = ObjectValue;
						pTx->SetText(std::to_string(value));
						changed = true;
					}
					//Plus
					if (InputManager::GetInstance()->IsButtonTriggered(SDL_CONTROLLER_BUTTON_B) || InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_A))// && chPlus > 0)
					{
						if (pState->mtool[PLUS])
						{
							audio->Play(0);
							value += ObjectValue;
							pTx->SetText(std::to_string(value));
							changed = true;
						}
					}
					//Minus
					if (InputManager::GetInstance()->IsButtonTriggered(SDL_CONTROLLER_BUTTON_X) || InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_S))// && chMinus > 0)
					{
						if (pState->mtool[MINUS])
						{
							audio->Play(0);
							value -= ObjectValue;
							pTx->SetText(std::to_string(value));
							changed = true;
						}
					}
				}
			}
			if (changed)
			{
				pBoxTx->MakeInactive();
				Event pEvent(OPENHUDDOOR);
				pEvent.pObject = mpOwner;
				EVENT_MANAGER->BroadCastEventToSubscribers(&pEvent);
				mUsed = true;
			}
		}
	}
}

void Cloud::Clear()
{
	mEnable = false;
	mScale = mOffset = glm::vec2(0.0f);
	pTrans = nullptr;
	mUsed = false;
}

Component* Cloud::Create()
{
	return new Cloud();
}

