/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"

Light::Light() : Component(LIGHT)
{

}

Light::~Light()
{

}

void Light::Update()
{
	Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	mData.Position = mOffset * pTr->mScale + pTr->mPosition;
	mData.Distance = mDistFactor * pTr->mScale.x;
	RenderManager::GetInstance()->mLightQueue.push_back(mData);
}

void Light::Serialize(rapidjson::Value::Object obj)
{

	if (obj.HasMember("Offset"))
	{
		rapidjson::Value::Array offset = obj["Offset"].GetArray();
		mOffset.x = offset[0].GetFloat();
		mOffset.y = offset[1].GetFloat();
	}
	if (obj.HasMember("Angle"))
		mData.Angle = obj["Angle"].GetFloat();
	if (obj.HasMember("Spread"))
		mData.Spread = obj["Spread"].GetFloat();
	if (obj.HasMember("Distance"))
		mDistFactor = obj["Distance"].GetFloat();
}

void Light::HandleEvent(Event* pEvent)
{

}

void Light::DeSerialize(std::ofstream* file)
{
	*file << "{\n\"Type\":\"Button\",\n";
	*file << "\"Offset\":[" << mOffset.x << "," << mOffset.y << "],\n";
	*file << "\"Angle\":" << mData.Angle << ",\n";
	*file << "\"Spread\":" << mData.Spread << ",\n";
	*file << "\"Distance\":" << mDistFactor;
	*file << "\n},\n";
}

void Light::Clear()
{
	mData.Position = glm::vec2(0.0f);
	mOffset = glm::vec2(0.0f);
	mData.Angle = 0.0f;
	mData.Distance = 0.0f;
	mData.Spread = 0.0f;
}

Component* Light::Create()
{
	return new Light();
}
