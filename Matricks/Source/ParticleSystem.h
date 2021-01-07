/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Vidhi Soni
- End Header --------------------------------------------------------*/
#pragma once

class ParticleEmitter;

class ParticleSystem
{
public:
	void Init(Shader *shader);
	void Draw(const RenderData& obj);
	void Close();
	inline static ParticleSystem* GetInstance() { return mParticle; }
	inline static ParticleSystem* CreateInstance() { mParticle = new ParticleSystem(); return mParticle; }

public:
	static ParticleSystem *mParticle;

private:
	Shader *mShader;
};