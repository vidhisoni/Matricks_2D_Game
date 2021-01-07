/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"

ParallaxBg::ParallaxBg() : Component(PARALLAX)
{
	LastCamPosition = glm::vec2(0.0f);
}

ParallaxBg::~ParallaxBg()
{
}

void ParallaxBg::Update()
{
	RenderManager* pRm = RenderManager::GetInstance();
	if(!pRm->mCamera.IsCameraControl())
		LastCamPosition = pRm->mCamera.GetPosition();
}

void ParallaxBg::Serialize(rapidjson::Value::Object obj)
{
	rapidjson::Value::Array factors = obj["Factoring"].GetArray();
	for (unsigned int i = 0; i < factors.Size(); ++i)
	{
		mBackgrounds.push_back(std::make_pair(mpOwner->mChildObjects[i], factors[i].GetFloat()));
	}
}

void ParallaxBg::DeSerialize(std::ofstream* file)
{
	*file << "{\n\"Type\":\"Parallax\",\n";
	*file << "\"Factoring\":[";
	for (auto b : mBackgrounds)
	{
		*file << std::get<1>(b) << ",";
	}
	long pos = (long)file->tellp();
	file->seekp(pos - 1);
	*file << "]\n},\n";
}

void ParallaxBg::HandleEvent(Event* pEvent)
{
	if (pEvent->mType == CAMERAMOVE)
	{
		glm::vec2 camPosition = RenderManager::GetInstance()->mCamera.GetPosition();
		glm::vec2 dxdy = ((LastCamPosition - camPosition) * 1000.0f) / (float)FrameRateController::GetInstance()->GetFrameTime();
		for (unsigned int i = 0; i < mBackgrounds.size(); ++i)
		{
			float factor = std::get<1>(mBackgrounds[i]);
			Transform* bgTr = static_cast<Transform*>(std::get<0>(mBackgrounds[i])->GetComponent(TRANSFORM));
			bgTr->mPosition += (factor * dxdy * (float)FrameRateController::GetInstance()->GetFrameTime()) / 1000.0f;
		}
		LastCamPosition = camPosition;
	}
}

Component* ParallaxBg::Create()
{
	return new ParallaxBg();
}

void ParallaxBg::Clear()
{
	mBackgrounds.clear();
}

