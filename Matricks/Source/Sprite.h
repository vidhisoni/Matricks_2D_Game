/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Sidhant Tumma
Modified by: Vidhi Soni
- End Header --------------------------------------------------------*/
#pragma once

class Sprite : public Component
{
public:
	Sprite();
	~Sprite();

	void Update();

	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);

	Component* Create();
	void Clear();
	
	inline void SetTexture(Texture* Tex) { mData.mTexture = Tex; }
	inline void SetZCoord(float zCoord) { mRenderData.zCoord = zCoord; }
	inline float GetZCoord() { return mRenderData.zCoord; }
	inline glm::vec2 GetTiles() { return mData.Tiles; }
	inline void SetTiles(glm::vec2 tiles) { mData.Tiles = tiles; }
	inline std::string ReturnTextype() { return mTexType; }
	inline void SetTextData(TextData* tData) { mRenderData.mText = tData; }
	inline void SetParticleData(ParticleEmitter* pData) { mRenderData.mParticle = pData; }
	inline Texture* GetTexture() { return mData.mTexture; }

	//TODO (sai) 
	bool changeTex;
	Texture *tempTex;
	RenderData mRenderData;
	SpriteData mData;

private:
	std::string mTexType;
	unsigned int mShape;
};