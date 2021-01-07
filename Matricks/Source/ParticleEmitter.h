/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Vidhi Soni
- End Header --------------------------------------------------------*/
#pragma once
#include "Component.h"
class Component;
class ParticleEmitter;
struct Particle {
	glm::vec2 position, velocity;
	glm::vec4 color;
	glm::vec2 direction;
	GLfloat life;
	GLfloat time;
	float scale;
	float rotation;
	float duration;
	float fade;
	float speed;
	Particle();
	void Reset();
};

class ParticleEmitter : public Component
{
public:
	ParticleEmitter();
	Component* Create();
	void Clear();
	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);
	std::deque<Particle>mParticles;
	std::deque<Particle>mDeleteParticles;
	void HandleEvent(Event* pEvent);
	GLuint mAmount;
	Particle p;
	//bool once = true;
	Texture *mTexture;
	Texture *mTexture_;

private:
	void Emit();
	//Shader *mShader;
	std::string TexType;
	glm::vec2 offset;
	int particleCount;
};