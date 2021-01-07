// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Sidhant Tumma
//- End Header --------------------------------------------------------
#include "stdafx.h"
#include <SDL.h>

Engine::Engine() : appIsRunning(true), pWindow(nullptr)
{}

Engine::~Engine()
{}

void Engine::Init()
{
	int error = 0;

	//Initialize SDL
	if ((error = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER)) < 0)//Init for video and joystick
	{
		printf("Couldn't initialize SDL, error %i\n", error);
		return;
	}
	//printf("%i joysticks were found.\n\n", SDL_NumJoysticks());//NUmber of joysticks
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Enable AntiAliasiing (Don't know what does this do)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

#ifdef RELEASE
	pWindow = SDL_CreateWindow("Matricks",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1920,
		1080,
		SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
		mFullscreen = true;
#elif DEBUG
	pWindow = SDL_CreateWindow("Matricks",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		720,
		SDL_WINDOW_OPENGL);
		mFullscreen = false;
#endif
	//Check if window was made successfully
	if (pWindow == NULL)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		return;
	}

	GetWindowSize();

	glContext = SDL_GL_CreateContext(pWindow);
	if (glContext == NULL)
	{
		printf("OpenGL context could not be created. SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		InitManagers();
	}

#ifdef DEBUG
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(pWindow, glContext);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
#endif
	mMinimized = false;
}

float accumulator = 0.0f;
void Engine::Run()
{
	GAMESTATE_MANAGER->LoadLevel(MAINMENU);
	while (appIsRunning)
	{
		
		InputManager::GetInstance()->Update();

		if (mMinimized)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		else
		{
			FrameRateController::GetInstance()->FrameStart();
			accumulator += FrameRateController::GetInstance()->GetFrameTime() / 1000.0f;
			if (accumulator > 0.2f)
				accumulator = 0.2f;
			/*ScriptHandler::GetInstance()->Update();*/
#ifdef DEBUG
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL2_NewFrame(pWindow);
			ImGui::NewFrame();
			{
				ImGui::Begin("Engine");                          // Create a window called "Hello, world!" and append into it.
				ImGui::Text("W A S D to move the camera\n");
				ImGui::Text("Arrow Keys to move the player\n");
				ImGui::Text("Space to jump(double jump is enabled)");
				ImGui::Text("Application average %.2f ms/frame (%.2f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::Text("Application average %d ms/frame (%.2f FPS)", FrameRateController::GetInstance()->GetFrameTime(), 1000.0f / FrameRateController::GetInstance()->GetFrameTime());
				ImGui::Text("accu %f: ", accumulator);
				ImGui::Text("");
				ImGui::Text("Memory");
				ImGui::Text("Active GameObjects %d", MemoryManager::GetInstance()->ActiveGameObjects());
				ImGui::Text("GameObjects In Memory %d", MemoryManager::GetInstance()->GameObjectsInMemory());
				//ImGui::Checkbox("Property Editor", &GameObjectManager::GetInstance()->buttonToggle);
				ImGui::Checkbox("Level Editor", &GameObjectManager::GetInstance()->ObjectDetials);
				ImGui::End();
			}
#endif
			bool test = GameObjectManager::GetInstance()->ObjectDetials;
			float deltaTime = (float)FrameRateController::GetInstance()->GetFrameTime();
			EventManager::GetInstance()->Update(deltaTime);
			float dt = PhysicsManager::GetInstance()->ReturnFrameTime();//so that this is only called once in a frame
			if (!test) {
				while (accumulator - dt >= dt)
				{
					PhysicsManager::GetInstance()->Update();
					accumulator -= dt;
				}

				PhysicsManager::GetInstance()->StateSetter(accumulator / dt);//(alpha) to solve any difference btw rendering and physics update
			}
#ifdef DEBUG
			GameObject* Player = GameObjectManager::GetInstance()->GetPlayer();
			if (Player != NULL)
			{
				Body *PlayerBody = static_cast<Body*>(Player->GetComponent(BODY));
				Transform* PlayerTransform = static_cast<Transform*>(Player->GetComponent(TRANSFORM));

				if (PlayerBody != NULL)
				{
					ImGui::Begin("Player Body");
					{
						ImGui::Text("Current Center Position %f %f", PlayerBody->ReturnPos().x, PlayerBody->ReturnPos().y);
						ImGui::Text("Current Transform Position %f %f", PlayerTransform->mPosition.x, PlayerTransform->mPosition.y);
						ImGui::Text("Current Velocity %f %f", PlayerBody->ReturnVelocity().x, PlayerBody->ReturnVelocity().y);
						ImGui::Text("Current Acceleration %f %f", PlayerBody->ReturnAcceleration().x, PlayerBody->ReturnAcceleration().y);
						ImGui::Text("Current Total force %f %f", PlayerBody->ReturnTotalForce().x, PlayerBody->ReturnTotalForce().y);
						ImGui::Text("JUMP %d", PlayerBody->rReturnJumpC());
					}
					ImGui::End();
				}
			}
#endif
			GameObjectManager::GetInstance()->Update();

			RenderManager::GetInstance()->Update();

			AudioManager::GetInstance()->Update(deltaTime);

			if (GameStateManager::GetInstance()->startScript) {
				ScriptHandler::GetInstance()->Update();
				GameStateManager::GetInstance()->startScript = false;
			}
#ifdef DEBUG
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
			SDL_GL_SwapWindow(pWindow);
			GameStateManager::GetInstance()->Update();
			GameObjectManager::GetInstance()->DestoryQ();
			FrameRateController::GetInstance()->FrameEnd();
		}
	}
}

void Engine::Close()
{
	RenderManager::GetInstance()->Close();
	AudioManager::GetInstance()->Close();
	InputManager::GetInstance()->Close();
	GameStateManager::GetInstance()->Close();
	FrameRateController::GetInstance()->Close();
	GameObjectManager::GetInstance()->Close();
	MemoryManager::GetInstance()->Close();
	PhysicsManager::GetInstance()->Close();
	ResourceManager::GetInstance()->Close();
	GameObjectFactory::GetInstance()->Close();
	CollisionManager::GetInstance()->Close();
	EventManager::GetInstance()->Close();
	ScriptHandler::GetInstance()->Close();

#ifdef DEBUG
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif

	// Close and destroy the window
	SDL_DestroyWindow(pWindow);

	// Quit SDL subsystems
	SDL_Quit();
}

void Engine::HideCursor()
{
	SDL_ShowCursor(SDL_DISABLE);
	mCursorVisible = false;
}

void Engine::ShowCursor()
{
	SDL_ShowCursor(SDL_ENABLE);
	mCursorVisible = true;
}

void Engine::Pause()
{
	if (!isPaused)
	{
		isPaused = true;
		GAMEOBJECT_MANAGER->Pause();
		ShowCursor();
	}
	AudioManager::GetInstance()->pauseAudio();
}

void Engine::UnPause()
{
	if (isPaused)
	{
		GAMEOBJECT_MANAGER->EmptyMenuStack();
		isPaused = false;
		GAMEOBJECT_MANAGER->UnPause();
		HideCursor();
	}
	AudioManager::GetInstance()->unpauseAudio();
}

glm::vec2 Engine::GetWindowSize()
{
	int x, y;
	SDL_GetWindowSize(pWindow, &x, &y);
	mWindowSize = glm::vec2(x, y);
	return mWindowSize;
}

void Engine::ChangeResolution(glm::vec2 size)
{
	mWindowSize = size;
	SDL_SetWindowSize(pWindow, mWindowSize.x, mWindowSize.y);
	SDL_SetWindowPosition(pWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	RENDER_MANAGER->ResolutionChanged();
}

void Engine::ChangetoFullScreen()
{
	SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	RENDER_MANAGER->ResolutionChanged();
	mFullscreen = true;
}

void Engine::ChangetoWindowed()
{
	SDL_SetWindowFullscreen(pWindow, 0);
	SDL_SetWindowPosition(pWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	RENDER_MANAGER->ResolutionChanged();
	mFullscreen = false;
}

void Engine::InitManagers()
{
	InputManager::CreateInstance()->Init();
	RenderManager::CreateInstance()->Init();
	AudioManager::CreateInstance()->Init();
	GameStateManager::CreateInstance()->Init();
	EventManager::CreateInstance()->Init();
	FrameRateController::CreateInstance()->Init();
	FrameRateController::GetInstance()->SetMaxFrameRate(60);
	GameObjectManager::CreateInstance()->Init();
	MemoryManager::CreateInstance()->Init();
	PhysicsManager::CreateInstance()->Init();
	PhysicsManager::GetInstance()->SetFPS(60.0f);
	ResourceManager::CreateInstance()->Init();
	GameObjectFactory::CreateInstance()->Init();
	CollisionManager::CreateInstance()->Init();
	ScriptHandler::CreateInstance()->Init();
}