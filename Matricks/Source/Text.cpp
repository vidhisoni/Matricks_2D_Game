/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
Modified by: Vidhi Soni
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include"SDL_keyboard.h"
#include"GameObjectManager.h"
#include"GameObjectFactory.h"
#include"Body.h"

Text::Text() : Component(TEXT)
{
	mValue = 0;
	mData.Color = glm::vec3(0.0f);
	mData.Scale = mData.xPos = mData.yPos = 0.0f;
	mData.Text = "";
	mOffset = glm::vec2(0.0f);
	isDirty = false;
}

void Text::Update()
{
	Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	mData.xPos = pTr->mPosition.x + mOffset.x * pTr->mScale.x;
	mData.yPos = pTr->mPosition.y + mOffset.y * pTr->mScale.y;
}

void Text::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("Value"))
	{
		//For Now Text
		//mValue = (int)obj[L"Value"]->AsNumber();
		//mData.Text = std::to_string(mValue);
		mData.Text = obj["Value"].GetString();
		if (mData.Text == "HACK") {
			int temp = (int)AudioManager::GetInstance()->Audio_Volume;
			mData.Text = std::to_string(temp);
		}
		if (mData.Text == "HACK1") {
			int temp = (int)AudioManager::GetInstance()->SFX_Volume;
			mData.Text = std::to_string(temp);
		}
	}
	if (obj.HasMember("Scale"))
		mData.Scale = obj["Scale"].GetFloat();
	if (obj.HasMember("Color"))
	{
		rapidjson::Value::Array clr = obj["Color"].GetArray();
		mData.Color = glm::vec3(clr[0].GetFloat(), clr[1].GetFloat(), clr[2].GetFloat());
	}
	if (obj.HasMember("Offset"))
	{
		rapidjson::Value::Array offset = obj["Offset"].GetArray();
		mOffset.x = offset[0].GetFloat();
		mOffset.y = offset[1].GetFloat();
	}
	Sprite* pSp = static_cast<Sprite*>(mpOwner->GetComponent(SPRITE));
	pSp->SetTextData(&mData);
	mActive = true;
}

void Text::DeSerialize(std::ofstream* file)
{
	*file << "\"Text\":{";
	*file << "\"Value\":\"" << mData.Text << "\",";
	*file << "\"Scale\":" << mData.Scale << ",";
	*file << "\"Color\":[" << mData.Color.x << "," << mData.Color.y << "," << mData.Color.z << "],";
	*file << "\"Offset\":[" << mOffset.x << "," << mOffset.y << "]";
	*file << "},";
}

void Text::Clear()
{
	mValue = 0;
	mOffset = glm::vec2(0.0f);
	mData.Color = glm::vec3(0.0f);
	mData.Scale = 0.0f;
	mData.Text = "";
	mActive = false;
	mData.xPos = mData.yPos = 0.0f;
}

Component* Text::Create()
{
	return new Text();
}

void Text::HandleEvent(Event* pEvent)
{
	if (pEvent->mType == INTERMEDIATEDOOR && mpOwner->GetType()==IDOOR && (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_R)||InputManager::GetInstance()->IsButtonTriggered(SDL_CONTROLLER_BUTTON_Y)))
	{
		Text* player = static_cast<Text*>(pEvent->pObject->mChildObjects[0]->GetComponent(TEXT));
		if (player == nullptr)
			return;
		Audio* audio = static_cast<Audio*>(pEvent->pObject->GetComponent(AUDIO));
		if (audio == nullptr)
			return;
		int playerValue = std::stoi(player->GetText());
		int objValue = std::stoi(mData.Text);
		if (objValue == playerValue && !mpOwner->mChildObjects.empty())
		{
			GameObject* obj=GameObjectFactory::GetInstance()->LoadObject("platform.json");
			if (obj == nullptr)
				return;
			Transform* ptr = static_cast<Transform*>(obj->GetComponent(TRANSFORM));
			if (ptr == nullptr)
				return;
			Transform* childPtr = static_cast<Transform*>(mpOwner->mChildObjects[0]->GetComponent(TRANSFORM));
			if (childPtr == nullptr)
				return;
			Body* pBody= static_cast<Body*>(obj->GetComponent(BODY));
			if (pBody == nullptr)
				return;
			ptr->mPosition = childPtr->mPosition;
			ptr->mAngle = childPtr->mAngle;
			ptr->mScale = childPtr->mScale;

			pBody->Initialize();
			
			Sprite* pSo= static_cast<Sprite*>(obj->GetComponent(SPRITE));
			if (pSo == nullptr)
				return;
			Sprite* pSp = static_cast<Sprite*>(mpOwner->mChildObjects[0]->GetComponent(SPRITE));
			if (pSp == nullptr)
				return;
			pSo->SetTiles(pSp->GetTiles());
			mpOwner->ChildFind(INVISIPLATFORM)->Destroy();
			audio->Play(1);
		}
	}
	if (pEvent->mType == LEVELCOMPLETE && mpOwner->GetType() == FINALDOOR)
	{
		Audio* audio = static_cast<Audio*>(pEvent->pObject->GetComponent(AUDIO));
		if (audio == nullptr)
			return;
		if (isDirty==false)
		{
			
			bool Complete = false;
			if (mData.Text == "")
			{
				audio->StopSFX(0);
				audio->Play(1);
				Controller *cont = static_cast<Controller *>(pEvent->pObject->GetComponent(CONTROLLER));
				cont->test = 0;
				RENDER_MANAGER->mCamera.ZoomCamera(0, 1.5f, static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM))->mPosition);
				static_cast<Animator*>(mpOwner->GetComponent(ANIMATOR))->PlayAnimation("opening", CHANGELEVEL);
				isDirty = true;
				mpOwner->emitParticle = true;
				Complete = true;
			}
			else
			{
				Text* player = static_cast<Text*>(pEvent->pObject->mChildObjects[0]->GetComponent(TEXT));
				if (player == nullptr)
					return;
				if (player->mData.Text == "")
					return;
				int playerValue = std::stoi(player->GetText());
				int objValue = std::stoi(mData.Text);
				if (objValue == playerValue)
				{
					Complete = true;
				}
			}
			if (Complete)
			{
				mData.Text = "";
				Controller *cont = static_cast<Controller *>(pEvent->pObject->GetComponent(CONTROLLER));
				cont->test = 0;
				RENDER_MANAGER->mCamera.ZoomCamera(0, 1.5f, static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM))->mPosition);
				if (pEvent->mContact->colSide == RIGHT) //Player's left
				{
					static_cast<Animator*>(mpOwner->GetComponent(ANIMATOR))->PlayAnimation("openingright", CHANGELEVEL);
					audio->StopSFX(0);
					audio->Play(1);
					isDirty = true;
					mpOwner->emitParticle = true;
				}
				else {
					static_cast<Animator*>(mpOwner->GetComponent(ANIMATOR))->PlayAnimation("openingleft", CHANGELEVEL);
					audio->StopSFX(0);
					audio->Play(1);
					isDirty = true;
					mpOwner->emitParticle = true;
				}
			}
		}
	}
	if (pEvent->mType == CHANGELEVEL)
	{
	//	if (!isDirty)
		//{
			GAMESTATE_MANAGER->AddCompletedLevel(GAMESTATE_MANAGER->GetCurrentLevel());
			GameStateManager::GetInstance()->NextLevel();
			//isDirty = true;
	//	}	
	}
	if (pEvent->mType == DIVIDEBOX)
	{
		Body* pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));
		if (pBody == nullptr)
			return;
		Audio* audio = static_cast<Audio*>(mpOwner->GetComponent(AUDIO));
		if (audio == nullptr)
			return;

		if (fabs(pBody->ReturnAcceleration().y - gravity.y) > 0.2f)
		{
			GameObject* obj1 = GameObjectFactory::GetInstance()->LoadObject("Box.json");
			GameObject* obj2 = GameObjectFactory::GetInstance()->LoadObject("Box.json");
			if (obj1 == nullptr || obj2 == nullptr)
				return;
			Transform* pTb = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
			Transform* ptr1 = static_cast<Transform*>(obj1->GetComponent(TRANSFORM));
			Transform* ptr2 = static_cast<Transform*>(obj2->GetComponent(TRANSFORM));
			if (pTb == nullptr || ptr1 == nullptr || ptr2 == nullptr)
				return;
			ptr1->mPosition = pTb->mPosition - glm::vec2(150.0f, 0.0f);
			ptr1->mAngle = pTb->mAngle;
			ptr1->mScale = pTb->mScale;

			ptr2->mPosition = pTb->mPosition + glm::vec2(150.0f, 0.0f);
			ptr2->mAngle = pTb->mAngle;
			ptr2->mScale = pTb->mScale;

			Body* pBody1 = static_cast<Body*>(obj1->GetComponent(BODY));
			if (pBody1 == nullptr)
				return;
			pBody1->Initialize();
			Body* pBody2 = static_cast<Body*>(obj2->GetComponent(BODY));
			if (pBody2 == nullptr)
				return;
			pBody2->Initialize();

			Sprite* pSp = static_cast<Sprite*>(mpOwner->GetComponent(SPRITE));
			Sprite* pSp1 = static_cast<Sprite*>(obj1->GetComponent(SPRITE));
			Sprite* pSp2 = static_cast<Sprite*>(obj2->GetComponent(SPRITE));
			if (pSp == nullptr || pSp1 == nullptr || pSp2 == nullptr)
				return;
			pSp1->SetZCoord(pSp->GetZCoord());
			pSp2->SetZCoord(pSp->GetZCoord());

			//Set Text
			Text* pTx1 = static_cast<Text*>(obj1->GetComponent(TEXT));
			Text* pTx2 = static_cast<Text*>(obj2->GetComponent(TEXT));
			if (pTx1 == nullptr || pTx2 == nullptr)
				return;
			int bigbox = GetValue();
			int val1, val2;
			if (bigbox % 2 == 0)
			{
				val1 = val2 = bigbox / 2;
			}
			else
			{
				val1 = bigbox / 2;
				val2 = (bigbox / 2) + 1;
			}
			pTx1->SetText(std::to_string(val1));
			pTx2->SetText(std::to_string(val2));
			mpOwner->Destroy();
			audio->Play();
		}
	}
	if (pEvent->mType == DELETEDOOR)
	{
		if (!mpOwner->mChildObjects.empty())
		{
			mpOwner->ChildFind(PLATFORM)->Destroy();
		}
	}
	if (pEvent->mType == CHARGEUSED)
	{
		MakeActive();
	}
	if (pEvent->mType == OPENHUDDOOR)
	{
		std::string pCloud = static_cast<Text*>(mpOwner->GetComponent(TEXT))->GetText();
		std::string pDoor = static_cast<Text*>(pEvent->pObject->GetComponent(TEXT))->GetText();
		int Cval = 0, Dval = 0;
		if (pCloud != "")
			Cval = std::stoi(pCloud);
		if (pDoor != "")
			Dval = std::stoi(pDoor);
		if (Cval == Dval)
			static_cast<Sprite*>(mpOwner->GetComponent(SPRITE))->SetTexture(RESOURCE_MANAGER->GetTexture("OpenHUDDoor"));
		else
			static_cast<Sprite*>(mpOwner->GetComponent(SPRITE))->SetTexture(RESOURCE_MANAGER->GetTexture("CloseHUDDoor"));
	}
}

void Text::MakeInactive()
{
	LogInfo("Inactive");
	Sprite* pSp = static_cast<Sprite*>(mpOwner->GetComponent(SPRITE));
	pSp->SetTexture(RESOURCE_MANAGER->GetTexture("inactivebox"));
	pSp->mData.Bloom = false;
	mActive = false;
	mpOwner->emitParticle = true;
}

void Text::MakeActive()
{
	LogInfo("Active");
	Sprite* pSp = static_cast<Sprite*>(mpOwner->GetComponent(SPRITE));
	pSp->SetTexture(RESOURCE_MANAGER->GetTexture("box"));
	pSp->mData.Bloom = true;
	mActive = true;
}

