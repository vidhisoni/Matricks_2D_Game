/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Vidhi Soni
Modified by: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "SDL.h"

extern Engine* engine;
RenderManager* RenderManager::pObject = nullptr;


RenderManager* RenderManager::CreateInstance()
{
	pObject = new RenderManager();
	return pObject;
}

void RenderManager::Init()
{
	DebugDraw = false;

	//Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		printf("Error initializing GLEW\n");
	}

	if (!GLEW_VERSION_4_3)
	{
		printf("OpenGL 4.3 is not supported\n");
	}

	//Opengl flags
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//Initialize Models
	ModelsInit();

	//Shader
	mShader = new Shader("Resources/Shaders/Shader.vs", "Resources/Shaders/Shader.fs");
	mTextShader = new Shader("Resources/Shaders/TextRenderer.vs", "Resources/Shaders/TextRenderer.fs");
	mParticleShader = new Shader("Resources/Shaders/ParticleSystem.vs", "Resources/Shaders/ParticleSystem.fs");
	mPortalShader = new Shader("Resources/Shaders/Door.vs", "Resources/Shaders/Door.fs");

	//Bloom shaders
	mBlurShader = new Shader("Resources/Shaders/Bloom.vs", "Resources/Shaders/Blur.fs");
	mBlurShader->Bind();
	mBlurShader->SetUniform1i("scene", 0);
	mBloomShader = new Shader("Resources/Shaders/Bloom.vs", "Resources/Shaders/Bloom.fs");
	mBloomShader->Bind();
	mBloomShader->SetUniform1i("scene", 0);
	mBloomShader->SetUniform1i("blur", 1);

#ifdef DEBUG
	//Debug Shader
	mDebugShader = new Shader("Resources/Shaders/Debug.vs", "Resources/Shaders/Debug.fs");
#endif

	//Projection Matrix
	mProjection = glm::ortho(0.0f, ENGINE_WIDTH, 0.0f, ENGINE_HEIGHT);
	
	//Normal Shader
	mShader->Bind();
	mShader->SetUniformMat4fLoc(4, mProjection);

	//Text Renderer
	mTextShader->Bind();
	mTextShader->SetUniformMat4fLoc(1,mProjection);
	mTextRenderer.Init(mTextShader);

	mAmbientLight = 0.3f;
	
	mParticleShader->Bind();
	//mParticleShader->SetUniformMat4fLoc(4, mProjection);
	//mParticleShader->SetUniformMat4fLoc(3, mCamera.mView);
	ParticleSystem::CreateInstance()->Init(mParticleShader);

	//Loading Screen
	LoadScreen();

	//Framebuffers for Bloom
	SceneFBO = new FrameBuffer(engine->GetWindowSize().x, engine->GetWindowSize().y, 2);

	BlurFBO[0] = new FrameBuffer(engine->GetWindowSize().x, engine->GetWindowSize().y, 1);
	BlurFBO[1] = new FrameBuffer(engine->GetWindowSize().x, engine->GetWindowSize().y, 1);

	BloomEnabled = true;

	mPortalShader->Bind();
	mPortalShader->SetUniformMat4fLoc(4, mProjection);
//	mPortalShader->SetUniform1i("mTexture", 0);

	//PortalEnabled = true;

	//Fade in-out
	mFade = -1;
	mFadeValue = 0.0f;

}

void RenderManager::Update()
{
	PreRendering();
	Rendering();
	PostRendering();
}

void RenderManager::Close()
{
	for (auto shape : mShapes)
	{
		std::get<0>(shape.second)->Delete();
		delete std::get<0>(shape.second);
		if (std::get<1>(shape.second) != nullptr)
		{
			std::get<1>(shape.second)->Delete();
			delete std::get<1>(shape.second);
		}
	}
	mShapes.clear();
	mShader->Unbind();
	delete mShader;
	delete mTextShader;
	delete mBloomShader;
	delete mBlurShader;
	delete SceneFBO;
	delete BlurFBO[0];
	delete BlurFBO[1];
	delete mPortalShader;
#ifdef DEBUG
	delete mDebugShader;
#endif
	ParticleSystem::GetInstance()->Close();
	delete mParticleShader;
	delete LoadingScreen;
	delete pObject;
}

void RenderManager::ResolutionChanged()
{
	glm::vec2 size = engine->GetWindowSize();
	SceneFBO->ChangeSize(size.x, size.y);
	BlurFBO[0]->ChangeSize(size.x, size.y);
	BlurFBO[1]->ChangeSize(size.x, size.y);
}

void RenderManager::ZSorting()
{
	std::sort(mRenderQueue.begin(), mRenderQueue.end(), [](RenderData obj1, RenderData obj2) {return obj1.zCoord > obj2.zCoord; });
}

void RenderManager::PreRendering()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ZSorting();

#ifdef DEBUG
	ImGui::Begin("Debug Tools");
	if (ImGui::Button("DebugDraw"))
		DebugDraw = !DebugDraw;
	ImGui::SameLine();
	ImGui::Text(" %s", DebugDraw ? "ON" : "OFF");
	//ImGui::SliderFloat("float", &mAmbientLight, 0.0f, 1.0f);
	ImGui::InputFloat("mAmbientLight", &mAmbientLight);
	ImGui::Text("Cloud Value");
	ImGui::SameLine();
	GameObject* player = GameObjectManager::GetInstance()->GetPlayer();

	/*if (player)
	{
		Text* pval = static_cast<Text*>(player->mChildObjects[0]->GetComponent(TEXT));
		int value = pval->GetValue();
		if (ImGui::Button("+"))
		{
			pval->SetText(std::to_string(value + 1));
		}
		ImGui::SameLine();
		if (ImGui::Button("-"))
		{
			pval->SetText(std::to_string(value - 1));
		}
	}*/
	ImGui::End();
#endif
	   	
	//Setting light uniforms (Futher optimization: Update uniforms only for changed lights)
	mShader->Bind();
	unsigned int lights = mLightQueue.size();
	mShader->SetUniform1i("lightnum", lights);
	for (unsigned int i = 0; i < lights; ++i)
	{
		float angle = mLightQueue[i].Angle * 0.01745f;
		mShader->SetUniform2f("lightpos[" + std::to_string(i) + "].pos", mLightQueue[i].Position.x, mLightQueue[i].Position.y);
		mShader->SetUniform3f("lightpos[" + std::to_string(i) + "].direction", 
			0.0f * cos(angle) + 1.0f * sin(angle), 0.0f * sin(angle) - 1.0f * cos(angle),0.0f);
		mShader->SetUniform1f("lightpos[" + std::to_string(i) + "].spread", mLightQueue[i].Spread);
		mShader->SetUniform1f("lightpos[" + std::to_string(i) + "].distance", mLightQueue[i].Distance);
	}
	mLightQueue.clear();
	mShader->SetUniform1f("Ambient", mAmbientLight);

	if (!engine->IsPaused())
	{//Fade in-out
		if (mFade == 0) // 0.0 - 1.0
		{
			mFadeValue += (FRAMERATE_CONTROLLER->GetFrameTime() / 1000.0f);
			if (mFadeValue > 0.9f)
			{
				mFade = -1;
				mFadeValue = 1.0f;
			}
		}
		else if (mFade == 1) // 1.0 - 0.0
		{
			mFadeValue -= (FRAMERATE_CONTROLLER->GetFrameTime() / 1000.0f);
			if (mFadeValue < 0.1f)
			{
				mFade = -1;
				mFadeValue = 0.0f;
			}
		}
		mTextShader->Bind();
		mTextShader->SetUniform1i("Fade", mFade);
		mTextShader->SetUniform1f("FadeValue", mFadeValue);
		mBloomShader->Bind();
		mBloomShader->SetUniform1i("Fade", mFade);
		mBloomShader->SetUniform1f("FadeValue", mFadeValue);
	}
}

//temp

float timeSinceAppStart = 0.0f;
//

void RenderManager::Rendering()
{
	if (INPUT_MANAGER->IsTriggered(SDL_SCANCODE_B))
		BloomEnabled = !BloomEnabled;

	/*if (INPUT_MANAGER->IsTriggered(SDL_SCANCODE_P))
		PortalEnabled = !PortalEnabled;*/

	mShader->Bind();
	if (BloomEnabled)
	{
		SceneFBO->Bind();
		mShader->SetUniform1i("BloomEnabled", 1);
	}
	else
	{
		SceneFBO->Unbind();
		mShader->SetUniform1i("BloomEnabled", 0);
	}

	for (const RenderData& obj : mRenderQueue)
	{
	
		auto mShader = this->mShader;// ->Bind();
		mShader->Bind();

		if (obj.mSprite->pTr->mpOwner->GetType() == PORTAL)
		{
			mShader = mPortalShader;
			mShader->Bind();
			//mPortalShader->Bind();
			obj.mSprite->mTexture->Bind();
			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(obj.mSprite->pTr->mPosition, 0.0f));
			mPortalShader->SetUniformMat4fLoc(2, model);
			mPortalShader->SetUniformMat4fLoc(3, mCamera.mView);
			mPortalShader->SetUniform3f("iResolution", 1280.0f,720.0f, 0);
			timeSinceAppStart += FrameRateController::GetInstance()->GetFrameTime() * 0.001;
			mPortalShader->SetUniform1f("iTime",  timeSinceAppStart);
		}
		
		else
		{
			if (obj.mSprite->isHUD)
			{
				mShader->SetUniform1i("EnableLighting", 0);
				mShader->SetUniform1i("BloomObject", 0);
				mShader->SetUniformMat4fLoc(3, glm::mat4(1.0f));
			}
			else
			{
				if (obj.mSprite->Bloom)
				{
					mShader->SetUniform1i("EnableLighting", 0);
					mShader->SetUniform1i("BloomObject", 1);
				}
				else
				{
					mShader->SetUniform1i("EnableLighting", 1);
					mShader->SetUniform1i("BloomObject", 0);
				}
				mShader->SetUniformMat4fLoc(3, mCamera.mView);
			}

		}

 		if (obj.mSprite)
		{
			mShader->SetUniform1i("mTexture", 0);
			obj.mSprite->mTexture->Bind();
		}

		mShader->SetUniformMat4fLoc(2, obj.mSprite->pTr->GetModel());
		mShader->SetUniform2f("Tile", obj.mSprite->Tiles.x, obj.mSprite->Tiles.y);

		
		//if (obj.mSprite->Portal)

		obj.VAO->Bind();
		if (obj.IBO != nullptr) //Draw Quad
		{
			obj.IBO->Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		else
			glDrawArrays(GL_TRIANGLES, 0, 3); //Draw Triangle

#ifdef DEBUG
		if (DebugDraw)
		{
			mDebugShader->Bind();
			mShader->SetUniformMat4fLoc(1, mProjection);
			if(obj.mSprite->isHUD)
				mShader->SetUniformMat4fLoc(2, glm::mat4(1.0));
			else
				mShader->SetUniformMat4fLoc(2, mCamera.mView);
			mShader->SetUniformMat4fLoc(3, obj.mSprite->pTr->GetDebugModel());
			if(obj.IBO != nullptr)
				glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, nullptr); //Draw Quad
			else
				glDrawArrays(GL_LINE_LOOP, 0, 3); //Draw Triangle
			mDebugShader->Unbind();
		}
#endif

		obj.VAO->Unbind();
		obj.IBO->Unbind();
		if(obj.mSprite->mTexture)
			obj.mSprite->mTexture->Unbind();

		//Render Text
		if (obj.mText)
			mTextRenderer.RenderText(obj.mText, obj.mSprite->isHUD);

		//if (obj.mSprite->pTr->mpOwner->GetComponent(PARTICLE))
		if(obj.mParticle)
		{
			mParticleShader->Bind();
			mParticleShader->SetUniformMat4fLoc(4, mProjection);
			mParticleShader->SetUniformMat4fLoc(3, mCamera.mView);
			//mParticleShader->SetUniformMat4fLoc(2, obj.mSprite->pTr->GetModel());

			ParticleSystem::GetInstance()->Draw(obj);
			mParticleShader->Unbind();
		}

	}
	mRenderQueue.clear();

	if (BloomEnabled)
		SceneFBO->Unbind();

	if(PortalEnabled)
		mPortalShader->Unbind();

#pragma region BLIT_TEX
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, hdrFBO);// checking Rendering FBO
	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glReadBuffer(GL_COLOR_ATTACHMENT1);
	//glBlitFramebuffer(0, 0, ENGINE_WIDTH, ENGINE_HEIGHT, 0, 0, ENGINE_WIDTH, ENGINE_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

}


void RenderManager::RenderQuad()
{
	mShapes[SCREEN_QUAD].first->Bind();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	mShapes[SCREEN_QUAD].first->Unbind();
}

void RenderManager::PostRendering()
{
	if(BloomEnabled)
	{

		bool horizontal = true, first_iteration = true;
		unsigned int amount = 20;
		mBlurShader->Bind();
		
		BlurFBO[1]->TexBind();
		BlurFBO[1]->Clear();
		for (unsigned int i = 0; i < amount; i++)
		{
			BlurFBO[horizontal]->Bind();
			mBlurShader->SetUniform1i("horizontal", horizontal);
			
			if (first_iteration)
				SceneFBO->TexBind(1,0);
			else
				BlurFBO[!horizontal]->TexBind();
			
			RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		BlurFBO[1]->Unbind();
		
		mBloomShader->Bind();
		SceneFBO->TexBind(0);
		BlurFBO[!horizontal]->TexBind(0,1);
		RenderQuad();
	}
	
	//Update Camera Position
	mCamera.Update();
}

void RenderManager::ModelsInit()
{
	//Quad
	float positions_quad[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
	};

	unsigned int indices_quad[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray* VAO_quad = new VertexArray();
	VertexBuffer VBO_quad(positions_quad, 4 * 4 * sizeof(float));
	IndexBuffer* IBO_quad =  new IndexBuffer(indices_quad, 6);

	VAO_quad->AddBuffer(VBO_quad);
	VAO_quad->Push(2, GL_FLOAT, sizeof(float));
	VAO_quad->Push(2, GL_FLOAT, sizeof(float));
	VAO_quad->AddLayout();
	IBO_quad->Bind();
	VBO_quad.Unbind();
	VAO_quad->Unbind();
 	mShapes[QUAD] = std::make_pair(VAO_quad, IBO_quad);

	//Triangle
	float positions_tri[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.0f,  0.5f, 1.0f, 1.0f
	};

	VertexArray* VAO_tri = new VertexArray();
	VertexBuffer VBO_tri(positions_tri, 4 * 4 * sizeof(float));
	
	VAO_tri->AddBuffer(VBO_tri);
	VAO_tri->Push(2, GL_FLOAT, sizeof(float));
	VAO_tri->Push(2, GL_FLOAT, sizeof(float));
	VAO_tri->AddLayout();
	VBO_tri.Unbind();
	VAO_tri->Unbind();
	mShapes[TRIANGLE] = std::make_pair(VAO_tri,nullptr);

	//Screen Quad
	float quadVertices[] = {
		// positions   // texture Coords
		-1.0f,  1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
	};

	// setup plane VAO
	VertexArray* VAO_screen = new VertexArray();
	VertexBuffer VBO_screen(quadVertices, 4 * 4 * sizeof(float));

	VAO_screen->AddBuffer(VBO_screen);
	VAO_screen->Push(2, GL_FLOAT, sizeof(float));
	VAO_screen->Push(2, GL_FLOAT, sizeof(float));
	VAO_screen->AddLayout();
	VBO_screen.Unbind();
	VAO_screen->Unbind();
	mShapes[SCREEN_QUAD] = std::make_pair(VAO_screen, nullptr);

}

void RenderManager::LoadScreen()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
	LoadingScreen = new Texture("Resources/Textures/DigipenLogo.png");
	mShader->Bind();
	mShader->SetUniform1i("mTexture", 0);
	glm::mat4 iden(1);
	mShader->SetUniformMat4fLoc(3, iden);
	glm::mat4 model;
	model = glm::translate(glm::mat4(1.0f), glm::vec3(ENGINE_WIDTH/2, ENGINE_HEIGHT/2, 0.0f));
	model = glm::scale(model, glm::vec3(ENGINE_WIDTH, ENGINE_HEIGHT, 0.0f));
	mShader->SetUniformMat4fLoc(2, model);
	mShader->SetUniform2f("Tile", 1.0f,1.0f);
	mShader->SetUniform1i("lightnum", 0);
	LoadingScreen->Bind();
	mShapes[QUAD].first->Bind();
	mShapes[QUAD].second->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	SDL_GL_SwapWindow(engine->GetWindow());
}
