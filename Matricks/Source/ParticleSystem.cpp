/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Vidhi Soni
- End Header --------------------------------------------------------*/
#include "stdafx.h"

#include "ParticleEmitter.h"
#include"SDL_keyboard.h"

ParticleSystem* ParticleSystem::mParticle = nullptr;

void ParticleSystem::Init(Shader *shader)
{
	mShader = shader;
}

void ParticleSystem::Draw(const RenderData& obj)
{
	obj.VAO->Bind();
	if(obj.IBO)
		obj.IBO->Bind();
	
	obj.mParticle->mTexture->Bind();
	mShader->SetUniform1i("sprite", 0);
	//for (ParticleEmitter *obj : mParticleQueue)
	//{
	for (Particle par : obj.mParticle->mParticles)
	{
		if (par.life > 0) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(par.position,0.0f));
			model = glm::scale(model, glm::vec3(par.scale));
			//model = glm::rotate(model , par.rotation, glm::vec3(1,1,0));
			//mShader->SetUniform2fvLoc(5, par.position);
			mShader->SetUniformMat4fLoc(2, model);

			//mShader->SetUniform1f("alpha", par.fade);
			//SEND ALPHA VALUE HERE TO FRAGMENT SHADER
			//mShader->SetUniform2f("offset", par.position.x, par.position.y);
			par.life -= FrameRateController::GetInstance()->GetFrameTime();
			mShader->SetUniform4f("ParticleColor", par.color.x, par.color.y, par.color.z, par.color.w);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
	}
	obj.VAO->Unbind();
	if (obj.IBO)
		obj.IBO->Unbind();


	obj.mParticle->mTexture->Unbind();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleSystem::Close()
{
	delete mParticle;
}
