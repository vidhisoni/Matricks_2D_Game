/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#include"stdafx.h"

Collider::Collider():Component(COLLIDER)
{
	mpShape = nullptr;
	mScaleFactor = glm::vec2(0.0f);
	mPosition = glm::vec2(0.0f);
	mScale = glm::vec2(0.0f);
}

Collider::~Collider()
{
	delete mpShape;
}

void Collider::Update()
{
}

void Collider::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("ScaleFactor")) {
		rapidjson::Value::Array  Scale = obj["ScaleFactor"].GetArray();
		mScaleFactor.x = Scale[0].GetFloat();
		mScaleFactor.y = Scale[1].GetFloat();
	}
	if (obj.HasMember("Position")) {
		rapidjson::Value::Array  Position = obj["Position"].GetArray();
		mPosition.x = Position[0].GetFloat();
		mPosition.y = Position[1].GetFloat();
	}
}

void Collider::DeSerialize(std::ofstream * file)
{
}

void Collider::HandleEvent(Event * pEvent)
{

}

void Collider::Clear()
{
}

void Collider::Initialize()
{
	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	if (pTr)
	{
		mPosition = pTr->mPosition;
		mScale = mScaleFactor * pTr->mScale;
	}
}

Component * Collider::Create()
{
	return new Collider();
}
