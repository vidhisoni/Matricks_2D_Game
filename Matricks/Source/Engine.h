// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Sidhant Tumma
//- End Header --------------------------------------------------------
#pragma once

struct SDL_Window;

#define ENGINE_WIDTH 1920.0f
#define ENGINE_HEIGHT 1080.0f
//Supported resolutions 1280×720, 1366×768, 1920×1080

//Managers Defines
#define AUDIO_MANAGER AudioManager::GetInstance()
#define COLLISION_MANAGER CollisionManager::GetInstance()
#define EVENT_MANAGER EventManager::GetInstance()
#define FRAMERATE_CONTROLLER FrameRateController::GetInstance()
#define GAMEOBJECT_FACTORY GameObjectFactory::GetInstance()
#define GAMEOBJECT_MANAGER GameObjectManager::GetInstance()
#define GAMESTATE_MANAGER GameStateManager::GetInstance()
#define INPUT_MANAGER InputManager::GetInstance()
#define MEMORY_MANAGER MemoryManager::GetInstance()
#define PHYSICS_MANAGER PhysicsManager::GetInstance()
#define RENDER_MANAGER RenderManager::GetInstance()
#define RESOURCE_MANAGER ResourceManager::GetInstance()
#define SCRIPT_ENGINE ScriptHandler::GetInstance()

class Engine
{
public:
	Engine();
	~Engine();
	void Init();
	void Run();
	void Close();

	inline void StopEngine() { appIsRunning = false; }
	inline bool IsRunning() { return appIsRunning; }
	inline SDL_Window* GetWindow() { return pWindow; }
	inline bool IsFullscreen() { return mFullscreen; }
	void HideCursor();
	void ShowCursor();
	inline bool IsCursorVisible() { return mCursorVisible; }
	void Pause();
	void UnPause();
	inline bool IsPaused() { return isPaused; }
	glm::vec2 GetWindowSize(); 
	void ChangeResolution(glm::vec2 size);
	void ChangetoFullScreen();
	void ChangetoWindowed();

	//TODO testing
	bool inSettings = false;
	bool mMinimized;

private:
	bool appIsRunning;
	bool isPaused;
	void InitManagers();
	SDL_Window *pWindow;
	glm::vec2 mWindowSize;
	void* glContext;
	bool mFullscreen;
	bool mCursorVisible;
};