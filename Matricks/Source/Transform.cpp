/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include <math.h>



Transform::Transform() : Component(TRANSFORM)
{

}


void Transform::Update()
{

}

glm::mat4 Transform::GetModel()
{	
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(mPosition.x, mPosition.y, 0.0f));
	model = glm::rotate(model, mAngle * radians, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(mScale.x, mScale.y, 1.0f));
	return model;
}

glm::mat4 Transform::GetDebugModel()
{
	Body *pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));
	if (pBody)
	{
		ShapeAABB *pAABB = static_cast<ShapeAABB*>(pBody->ReturnShape());//SHANTANU
		if (pAABB)
		{
			glm::mat4 dmodel = glm::translate(glm::mat4(1.0f), glm::vec3(pAABB->ReturnPos().x, pAABB->ReturnPos().y, 0.0f));
			dmodel = glm::scale(dmodel, glm::vec3(pAABB->ReturnExtent().x, pAABB->ReturnExtent().y, 0.0f));
			return dmodel;
		}
	}
	return glm::mat4(1.0f);
}

void Transform::HandleEvent(Event *pEvent)
{
	
}

void Transform::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("Pos")) {
		rapidjson::Value::Array Pos = obj["Pos"].GetArray();
		mPosition.x = Pos[0].GetFloat();
		mPosition.y = Pos[1].GetFloat();
		if (Pos.Size() == 3)
		{
			mZCoord = Pos[2].GetFloat();
			Sprite* pSp = static_cast<Sprite*>(mpOwner->GetComponent(SPRITE));
			if(pSp)
				pSp->SetZCoord(mZCoord);
		}
	}
	if (obj.HasMember("Scale")) {
		rapidjson::Value::Array Scale = obj["Scale"].GetArray();
		mScale = glm::vec2(Scale[0].GetFloat(), Scale[1].GetFloat());
		if (mpOwner->GetType() == NUMCLOUD)
		{
			Cloud* pCloud = static_cast<Cloud*>(mpOwner->GetComponent(CLOUD));
			if (pCloud)
				pCloud->SetScale(mScale);
		}
	}
	if (obj.HasMember("Angle"))
		mAngle = obj["Angle"].GetFloat();
}


void Transform::DeSerialize(std::ofstream* file)
{
	*file << "\"Transform\":{";
	*file << "\"Pos\":[" << mPosition.x << "," << mPosition.y << "," << mZCoord << "],";
	*file << "\"Scale\":[" << mScale.x << "," << mScale.y << "],";
	*file << "\"Angle\":" << mAngle << "";
	*file << "},";
}	

void Transform::CopyTransform(Transform* pTr)
{
	mPosition = pTr->mPosition;
	mAngle = pTr->mAngle;
	mScale = pTr->mScale;
	Body* pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));
	if (pBody)
		pBody->Initialize();
}

Component* Transform::Create()
{
	return new Transform();
}

void Transform::Clear()
{
	mPosition = glm::vec2(0.0f);
	mScale = glm::vec2(0.0f);
	mAngle = mZCoord = 0.0f;
}

