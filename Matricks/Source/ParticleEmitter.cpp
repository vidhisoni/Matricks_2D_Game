/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Vidhi Soni
- End Header --------------------------------------------------------*/

#include "stdafx.h"
#include "SDL_scancode.h"

GLuint lastUsedParticle = 0;

extern Engine* engine;

#include <random>
std::mt19937_64 RNGen;
std::uniform_real_distribution<> myrandomX(-50.0, 20.0);
std::uniform_real_distribution<> myrandomY(10.0, 20.0);
std::uniform_real_distribution<> myrandom1(200, 250);
std::uniform_real_distribution<> myrandomXBoxUp(-50, 50.0);
std::uniform_real_distribution<> myrandomYBoxUp(50.0, 180.0);
std::uniform_real_distribution<> myrandomConfettiX(-100.0, 100.0);
std::uniform_real_distribution<> myrandomConfettiY(100.0, 150.0);
std::uniform_real_distribution<> myrandomConfettiYUp(20.0, 50.0);
std::uniform_real_distribution<> myrandomConfettiXDown(-5.0, 5.0);
std::uniform_real_distribution<> myrandomConfettiYDown(5.0, 10.0);
std::uniform_real_distribution<>myrandColor(0.1, 0.7);
std::uniform_real_distribution<>myrandomDuration(10.0, 15.0);
std::uniform_real_distribution<>myrandomSpeedForConfetti(30.0, 35.0);
std::uniform_real_distribution<> myrandomScaleConfetti(150, 200);
std::uniform_real_distribution<> myrandomeRotation(-1, 1);
std::uniform_int_distribution<> myrandomIndex(0, 15);


//

struct Color
{
	float r, g, b;
	Color(float _r, float _g, float _b) {
		r = _r;
		g = _g;
		b = _b;
	}
};
Color blue1(176 / 255.0f, 196 / 255.0f, 222 / 255.0f);
Color blue2(30 / 255.0f, 144 / 255.0f, 255 / 255.0f) ;
Color blue3(100 / 255.0f, 149 / 255.0f, 237 / 255.0f);
Color blue4(70 / 255.0f, 130 / 255.0f, 180 / 255.0f) ;
Color blue5(95 / 255.0f, 158 / 255.0f, 160 / 255.0f) ;
Color blue6(123 / 255.0f, 104 / 255.0f, 238 / 255.0f);
Color blue7(106 / 255.0f, 90 / 255.0f, 205 / 255.0f) ;
Color blue8(72 / 255.0f, 61 / 255.0f, 139 / 255.0f)	 ;
Color blue9(65 / 255.0f, 105 / 255.0f, 225 / 255.0f) ;
Color blue10(0, 0, 255 / 255.0f)					 ;
Color blue11(0, 0, 205 / 255.0f)					 ;
Color blue12(0, 0, 139 / 255.0f)					 ;
Color blue13(0, 0, 128 / 255.0f)					 ;
Color blue14(25 / 255.0f, 25 / 255.0f, 112 / 255.0f) ;
Color blue15(138 / 255.0f, 43 / 255.0f, 226 / 255.0f);
Color blue16(75 / 255.0f, 0, 130 / 255.0f)			 ;

Color colorArray[] = { blue1,blue2,blue3,blue4,blue5,blue6,blue7,blue8,blue9,blue10,blue11,blue12,blue13,blue14,blue15,blue16 };
//
//std::uniform_real_distribution<> myrandomXScreen(0.0, 1280);
//std::uniform_real_distribution<> myrandomYScreen(0, 0);
//std::uniform_real_distribution<> myrandomXBoxLeft(-150.0, -100.0);
//std::uniform_real_distribution<> myrandomYBoxLeft(50.0, 150.0);
//std::uniform_real_distribution<> myrandomXBoxRight(-20.0, 30.0);
//std::uniform_real_distribution<> myrandomYBoxRight(50.0, 150.0);

#define LIFE_TIME 4

Particle::Particle()
{
	Reset();
}

void Particle::Reset()
{
	scale = myrandom1(RNGen);
	direction = glm::vec2(0.0f, 0.0f);
	position.x = 0.0f;
	position.y = 0.0f;
	velocity = glm::vec2(0.0f);
	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	fade = 1.0f;
	life = LIFE_TIME;
	time = 1.0f;
}

ParticleEmitter::ParticleEmitter() : Component(PARTICLE)
{
	particleCount = 0;
	srand(time(NULL));
}

Component * ParticleEmitter::Create()
{
	return new ParticleEmitter();
}

void ParticleEmitter::Clear()
{
	mTexture = nullptr;
	glm::vec2 offset = glm::vec2(0.0f);
	int particleCount = 0;
	mParticles.clear();
}

void ParticleEmitter::Update()
{
	if (engine->IsPaused())
		return;
	if (mpOwner->GetType() == PLAYER)
	{
		PlayerState *pState = static_cast<PlayerState*>(mpOwner->GetComponent(PLAYERSTATE));
		if (pState != NULL)
		{
			PlayerStates state = pState->GetCurrentState();
			Body *pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));

			if (state == MOVELEFT || state == MOVERIGHT)
			{
				if (particleCount < mAmount) {
					Emit();
					particleCount++;
				}

				std::vector<int> positions;
				int counter = 0;
				for (auto par = mParticles.begin(); par != mParticles.end(); ++par)
				{
					(*par).position.x -= 0.1f;// (float)rand() / RAND_MAX * 0.001;
					(*par).velocity += pBody->ReturnVelocity() * 0.001f;
					//par.position += par.velocity;
					//par.position.x -= 0.1;
					float ft = FrameRateController::GetInstance()->GetFrameTime() * 0.01;

					(*par).position.y++;
					//if (state == MOVELEFT)
				//{
				//	//(*par).velocity = -(*par).velocity;
				//	(*par).position.x = -(*par).position.x;
				//	(
				//}
				//adjust fade value here
					(*par).life -= ft;
					(*par).fade -= ft * 0.1f;
					if ((*par).life < 0)
					{
						positions.push_back(counter);
					}
					counter++;
				}

				//maybe use list or something else so that we are not removing
				//particles like this
				//remove finished particles
				for (auto& pos : positions) {
					particleCount--;
					if(mParticles.size() > pos)
					mParticles.erase(mParticles.begin() + pos);
				}
			}
			if (pBody->rReturnJumpC() || state == IDLELEFT || state == IDLERIGHT)
			{
				mParticles.clear();
				particleCount = 0;
			}
		}
	}
	else if (mpOwner->GetType() == NUMBOX)
	{ 
		
		if (mpOwner->emitParticle)
		{
			for (int i = 0; i < mAmount; ++i)
			{
				Emit();
			}
			mpOwner->emitParticle = false;
		}
		else
		{
			for (auto par = mParticles.begin(); par != mParticles.end(); ++par)
			{
				float ft = FrameRateController::GetInstance()->GetFrameTime()* 0.01;
				(*par).position.x -= 0.1f;// (float)rand() / RAND_MAX * 0.001;
				(*par).position.y += 2.0f;
				(*par).life -= ft;
				(*par).fade -= ft * 0.1f;
			}
		}
	}
	else if (mpOwner->GetType() == FINALDOOR)
	{
		if (mpOwner->emitParticle)
		{
			for (int i = 0; i < mAmount; ++i)
			{
				Emit();
			}
			mpOwner->emitParticle = false;
		}
		else
		{
			for (auto par = mParticles.begin(); par != mParticles.end(); ++par)
			{
				float ft = FrameRateController::GetInstance()->GetFrameTime()* 0.01;
				//(*par).position.x -= 0.1f;// (float)rand() / RAND_MAX * 0.001;
				//(*par).position.y -= 3.0f;
				//else
				
				/*if((*par).time > 10.0f)
				{
					(*par).position.x -= myrandomConfettiXDown(RNGen);
					(*par).position.y -= myrandomConfettiYDown(RNGen);
				}
				else*/
				//{
					//(*par).position.x = p.direction.x;
					//(*par).position.y = p.direction.y;//myrandomConfettiY(RNGen);
					//(*par).time += 0.5f;
				//}
				if ((*par).time < (*par).duration)
				{
				//	if ((*par).position.y < (*par).direction.y)
				//	{
				//		(*par).position.x += myrandomConfettiX(RNGen);
				//		(*par).position.y += myrandomConfettiYUp(RNGen);
				//	
					//else //if((*par).position.x > (*par).direction.x && (*par).position.y > (*par).direction.y)
					//{
					//	(*par).position.y -= myrandomConfettiYDown(RNGen) * 0.5f;
					//}


					(*par).position += (*par).direction * (*par).speed;
					(*par).time += 1.0f;
				}
				else
				{

				///	(*par).position.x += myrandomConfettiXDown(RNGen)* 0.4f;
					(*par).position.y -= myrandomConfettiYDown(RNGen) * 0.4f;
					//(*par).rotation += .05f *myrandomeRotation(RNGen)*ft;
				}
				
				(*par).life -= 0.2 * ft;
				(*par).fade -= ft * 0.1f;
			}
		}
	}
 }


void ParticleEmitter::Emit()
{	
	switch (mpOwner->GetType())
	{
		case PLAYER:
		{
			PlayerState *pState = static_cast<PlayerState*>(mpOwner->GetComponent(PLAYERSTATE));
			PlayerStates state = pState->GetCurrentState();
			glm::vec2 offsetTemp = offset;
			Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
			p.position.x = pTr->mPosition.x + myrandomX(RNGen);
			p.position.y = pTr->mPosition.y + myrandomY(RNGen);
			Body *pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));
			p.velocity += pBody->ReturnVelocity() * 0.001f;
			if (state == MOVELEFT)
			{
				//p.velocity = -p.velocity;
				//p.position.x = -p.position.x;
				offsetTemp.x += 20.0f;
			}
			//par.position += par.velocity;
			//p = par;
			else
			{
				offsetTemp.x -= 100.0f;
			}
			p.position += offsetTemp;
		}		
			break;
		case NUMBOX:
		{
			Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
			glm::vec2 offsetTemp = offset;
			//if (count < 20)
			{
				p.position.x = pTr->mPosition.x + myrandomXBoxUp(RNGen);
				p.position.y = pTr->mPosition.y + myrandomYBoxUp(RNGen);
				p.position += offsetTemp;
			}
			/*else if (count > 20 && count < 40)
			{
				p.position.x = pTr->mPosition.x + myrandomXBoxLeft(RNGen);
				p.position.y = pTr->mPosition.y + myrandomYBoxLeft(RNGen);
				p.velocity += pBody->ReturnVelocity() * 0.001f;
				p.position += offsetTemp;
			}
			else
			{
				p.position.x = pTr->mPosition.x + myrandomXBoxRight(RNGen);
				p.position.y = pTr->mPosition.y + myrandomYBoxRight(RNGen);
				p.velocity += pBody->ReturnVelocity() * 0.001f;
				p.position += offsetTemp;
			}*/
			
		}
			break;

		case FINALDOOR:
		{
			Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
			Body *pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));

			glm::vec2 offsetTemp = offset;
			p.position.x = pTr->mPosition.x;// +myrandomXBoxUp(RNGen);
			p.position.y = pTr->mPosition.y -pBody->ReturnShape()->GetExtents().y;
			//p.position += offsetTemp;
			p.direction.x =  pTr->mPosition.x + myrandomConfettiX(RNGen);
			p.direction.y = pTr->mPosition.y +myrandomConfettiY(RNGen);
			p.direction = glm::normalize(p.direction - p.position);
			p.speed = myrandomSpeedForConfetti(RNGen);
			p.duration = myrandomDuration(RNGen);
			/*Color c = colorArray[myrandomIndex(RNGen)];
			p.color.x = c.r;
			p.color.y = c.g;
			p.color.z = c.b;*/
			p.scale = myrandomScaleConfetti(RNGen);
		}
		break;

	}//p.life -= 1 * i;
	//if (!mParticles.size())
	mParticles.push_back(p);
}



void ParticleEmitter::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("Texture"))
	{
		TexType = obj["Texture"].GetString();
		mTexture = ResourceManager::GetInstance()->GetTexture(TexType);
	}
	if (obj.HasMember("Amount"))
	{
		mAmount = obj["Amount"].GetInt();
	}
	if (obj.HasMember("Offset")) {
		rapidjson::Value::Array Offset = obj["Offset"].GetArray();
		offset.x = Offset[0].GetFloat();
		offset.y = Offset[1].GetFloat();
	}
	Sprite* pSp = static_cast<Sprite*>(mpOwner->GetComponent(SPRITE));
	pSp->SetParticleData(this);
}

void ParticleEmitter::DeSerialize(std::ofstream* file)
{
	*file << "{\n\"Type\":\"Particle\",\n";
	*file << "\"Texture\":\"" << TexType << "\",\n";
	*file << "\"Amount\":" << mAmount << ",\n";
	*file << "\"Offset\":[" << offset.x << "," << offset.y << "]\n},\n";
}

void ParticleEmitter::HandleEvent(Event* pEvent)
{
	if (pEvent->mType == LEVELCOMPLETE && mpOwner->GetType() == PLAYER)
	{
		mParticles.clear();
		particleCount = 0;
	}
	if (mpOwner->GetType() == NUMBOX)
	{
		if (InputManager::GetInstance()->IsButtonTriggered(SDL_SCANCODE_S))
		{
			mTexture = ResourceManager::GetInstance()->GetTexture("minus");
		}
		if (InputManager::GetInstance()->IsButtonTriggered(SDL_SCANCODE_A))
		{
			mTexture = ResourceManager::GetInstance()->GetTexture("plus");
		}
		if(InputManager::GetInstance()->IsButtonTriggered(SDL_SCANCODE_E))
		{
			mTexture = ResourceManager::GetInstance()->GetTexture("pickup");
		}
	 }
}