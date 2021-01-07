/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Sidhant Tumma
Modified by: Vidhi Soni
- End Header --------------------------------------------------------*/
#include "stdafx.h"

Sprite::Sprite() : Component(SPRITE)
{
	Clear();
}

Sprite::~Sprite()
{
}

void Sprite::Update()
{
	RenderManager::GetInstance()->mRenderQueue.push_back(mRenderData);
}

void Sprite::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("Texture"))
	{
		mTexType = obj["Texture"].GetString();
		mData.mTexture = ResourceManager::GetInstance()->GetTexture(mTexType);
	}
	if (obj.HasMember("Shape"))
	{
		mShape = obj["Shape"].GetUint();
		mRenderData.VAO = std::get<0>(RenderManager::GetInstance()->mShapes[(SpriteShape)mShape]);
		mRenderData.IBO = std::get<1>(RenderManager::GetInstance()->mShapes[(SpriteShape)mShape]);
		mData.Tiles = glm::vec2(1.0f, 1.0f);
		mData.pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	}
	if (obj.HasMember("Tiling"))
	{
		mData.mTexture->EnableTiling();
		rapidjson::Value::Array tiles = obj["Tiling"].GetArray();
		mData.Tiles.x *= tiles[0].GetFloat();
		mData.Tiles.y *= tiles[1].GetFloat();
	}
	if (obj.HasMember("HUD"))
	{
		mData.isHUD = true;
	}
	if (obj.HasMember("Bloom"))
	{
		mData.Bloom = true;
	}
	/*if (obj.HasMember("Portal"))
	{
		mData.Portal = true;
	}*/
	if (mRenderData.zCoord == 0.0f)
	{
		Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		SetZCoord(pTr->mZCoord);
	}
	mRenderData.mSprite = &mData;
}

void Sprite::DeSerialize(std::ofstream* file)
{
	*file << "\"Sprite\":{";
	*file << "\"Texture\":\"" << mTexType << "\",";
	*file << "\"Shape\":" << mShape << "";
	if (mData.Tiles != glm::vec2(1.0f))
		*file << ",\"Tiling\":[" << mData.Tiles.x << "," << mData.Tiles.y << "]";
	if (mData.isHUD)
		*file << ",\"HUD\":true";
	if (mData.Bloom)
		*file << ",\"Bloom\":true";
	*file << "},";

}

Component * Sprite::Create()
{
	return new Sprite();
}

void Sprite::Clear()
{
	mData.mTexture = nullptr;
	mRenderData.IBO = nullptr;
	mRenderData.VAO = nullptr;
	mData.Tiles = glm::vec2(0.0f);
	mData.isHUD = false;
	mData.pTr = nullptr;
	mData.Bloom = false;
	mRenderData.zCoord = 0.0f;
	mRenderData.mText = nullptr;
	mRenderData.mSprite = nullptr;
	mRenderData.mParticle = nullptr;
	changeTex = false;
}


