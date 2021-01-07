/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
Modified by: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#include"stdafx.h"

#include "SDL.h"
#include "SDL_keyboard.h"
//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;
//Analog joystick dead zone

extern Engine *engine;

InputManager* InputManager::pObject = nullptr;

InputManager* InputManager::CreateInstance()
{
	pObject = new InputManager();
	return pObject;
}

void InputManager::Init()
{
	SDL_memset(mCurrentState, 0, 512 * sizeof(Uint8));
	SDL_memset(mPreviousState, 0, 512 * sizeof(Uint8));
	SDL_memset(mPrevMouseState, 0, 3 * sizeof(bool));
	SDL_memset(mCurrentMouseState, 0, 3 * sizeof(bool));
	SDL_memset(mKeyTime, 0, 512 * sizeof(Uint32));
	SDL_memset(mJoystickCurrent, 0, NUMBER * sizeof(int));
	SDL_memset(mJoystickPrevious, 0, NUMBER * sizeof(int));
	mGameController = SDL_GameControllerOpen(0);
	mControllerActive=false;
	if (mGameController != nullptr)
	{
		SDL_JoystickEventState(1);
		mControllerActive = true;
	}
}

void InputManager::Close()
{
	SDL_GameControllerClose(mGameController);
	mGameController = NULL;
	delete pObject;
}

void InputManager::Update()
{
	//fetch mouse state
	SDL_memcpy(mPrevMouseState, mCurrentMouseState, 3 * sizeof(bool));
	SDL_memcpy(mJoystickPrevious, mJoystickCurrent, NUMBER * sizeof(int));
	SDL_Event e;
	SDL_Event j;
	while (SDL_PollEvent(&e) != 0)
	{

#ifdef DEBUG
		ImGui_ImplSDL2_ProcessEvent(&e);
#endif

		//User requests quit
		if (e.type == SDL_WINDOWEVENT) {
			switch (e.window.event) {
			case SDL_WINDOWEVENT_MINIMIZED:
				if (GAMESTATE_MANAGER->GetCurrentLevel() > STORYLINE7)
				{
					engine->Pause();
					GAMESTATE_MANAGER->LoadInGameMenu(PAUSEMENU);
				}
				engine->mMinimized = true;
				break;
#ifdef RELEASE
				case SDL_WINDOWEVENT_FOCUS_LOST:
					if (GAMESTATE_MANAGER->GetCurrentLevel() > STORYLINE7)
					{
						engine->Pause();
						GAMESTATE_MANAGER->LoadInGameMenu(PAUSEMENU);
					}
					engine->mMinimized = true;
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					engine->mMinimized = false;
					break;
#endif
			case SDL_WINDOWEVENT_RESTORED:
				engine->mMinimized = false;
				break;
			}
		}
		if (engine->mMinimized)
			return;

		if (e.type == SDL_QUIT)
		{
			engine->Pause();
			GAMESTATE_MANAGER->LoadInGameMenu(QUITCONFIRM);
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			mCurrentMouseState[e.button.button - 1] = true;
		}
		if (e.type == SDL_MOUSEBUTTONUP)
		{
			mCurrentMouseState[e.button.button - 1] = false;
		}
		if (e.type == SDL_CONTROLLERBUTTONDOWN)
		{
			mJoystickCurrent[e.jbutton.button] = true;
		}
		if (e.type == SDL_MOUSEMOTION)
		{
			if (GAMESTATE_MANAGER->GetCurrentLevel() < STORYLINE1 || (engine->IsPaused() && GAMESTATE_MANAGER->GetCurrentLevel() > STORYLINE7))
				engine->ShowCursor();
		}
		//if (e.type == SDL_JOYAXISMOTION || e.type == SDL_JOYHATMOTION)//CHANGE MAYBE
		//{
		//	if (e.jaxis.axis == 0)
		//	{
		//		//Left of dead zone
		//		if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
		//		{
		//			mJoystickCurrent[SDL_CONTROLLER_AXIS_LEFTX] = -1;
		//		}
		//		//Right of dead zone
		//		else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
		//		{
		//			mJoystickCurrent[SDL_CONTROLLER_AXIS_LEFTX]= 1;
		//		}
		//		else
		//		{
		//			mJoystickCurrent[SDL_CONTROLLER_AXIS_LEFTX] = 0;
		//		}
		//	}
		//	if (e.jaxis.axis == 1)
		//	{
		//		//Left of dead zone
		//		if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
		//		{
		//			mJoystickCurrent[SDL_CONTROLLER_AXIS_LEFTY] = -1;
		//		}
		//		//Right of dead zone
		//		else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
		//		{
		//			mJoystickCurrent[SDL_CONTROLLER_AXIS_LEFTY] = 1;
		//		}
		//		else
		//		{
		//			mJoystickCurrent[SDL_CONTROLLER_AXIS_LEFTY] = 0;
		//		}
		//	}
		//}//CHANGE MAYBE
		if (e.type == SDL_CONTROLLERBUTTONUP)
		{
			mJoystickCurrent[e.jbutton.button] = false;
		}
	}
	//Pause Engine
	if (IsTriggered(SDL_SCANCODE_ESCAPE) && GAMESTATE_MANAGER->GetCurrentLevel() > MAINMENU && GAMESTATE_MANAGER->GetCurrentLevel() < CREDITS)
	{
		if (engine->IsPaused())
		{
			engine->UnPause();
		}
		else
		{
			engine->Pause();
			if(GAMESTATE_MANAGER->GetCurrentLevel() > STORYLINE7 &&  GAMESTATE_MANAGER->GetCurrentLevel() < ENDSTORY1)
				GAMESTATE_MANAGER->LoadInGameMenu(PAUSEMENU);
			else
				GAMESTATE_MANAGER->LoadInGameMenu(STORYPAUSE);
		}		
	}
	//Start Scripthandling
	if (IsTriggered(SDL_SCANCODE_L))
	{
		ScriptHandler::GetInstance()->Update();
	}
	//Next Level Cheat
	if (IsTriggered(SDL_SCANCODE_G))
	{
		GAMESTATE_MANAGER->NextLevel();
	}
	if (IsTriggered(SDL_SCANCODE_2))
	{
		GAMESTATE_MANAGER->ChangeLevel(LEVELLIST);
	}
	//Ending Cheat
	if ((IsPressed(SDL_SCANCODE_RCTRL) || IsPressed(SDL_SCANCODE_LCTRL)) && IsTriggered(SDL_SCANCODE_G))
	{
		GAMESTATE_MANAGER->ChangeLevel(ENDSTORY1);
	}
	if (IsTriggered(SDL_SCANCODE_F11))
	{
		if (engine->IsFullscreen())
		{
			engine->ChangeResolution(glm::vec2(1280, 720));
			engine->ChangetoWindowed();
		}
		else
			engine->ChangetoFullScreen();
	}
	
	//fetch keyboard state
	int numberOfFetchedKeys = 0;
	const Uint8* pCurrentKeyStates = SDL_GetKeyboardState(&numberOfFetchedKeys);

	if (numberOfFetchedKeys > 512)
	{
		numberOfFetchedKeys = 512;
	}
	SDL_JoystickEventState(1);
	SDL_memcpy(mPreviousState, mCurrentState, 512 * sizeof(Uint8));
	SDL_memcpy(mCurrentState, pCurrentKeyStates, numberOfFetchedKeys * sizeof(Uint8));
}

bool InputManager::IsPressed(unsigned int KeyScanCode)
{
	if (KeyScanCode >= 512)
		return false;

	if (mCurrentState[KeyScanCode])
	{
		mKeyTime[KeyScanCode]++;
		return true;
	}

	return false;
}

bool InputManager::IsTriggered(unsigned int KeyScanCode)
{
	if (KeyScanCode >= 512)
		return false;

	if (mCurrentState[KeyScanCode] && !mPreviousState[KeyScanCode])
	{
		mKeyTime[KeyScanCode] = 0;
		return true;
	}

	return false;
}

bool InputManager::IsReleased(unsigned int KeyScanCode)
{
	if (KeyScanCode >= 512)
		return false;

	if (!mCurrentState[KeyScanCode] && mPreviousState[KeyScanCode])
	{
		mKeyTime[KeyScanCode] = 0;
		return true;
	}
	return false;
}

bool InputManager::IsMouseClicked(unsigned int KeyScanCode)
{
	if (mCurrentMouseState[KeyScanCode - 1] && !mPrevMouseState[KeyScanCode - 1])
	{
		return true;
	}
	return false;
}

int InputManager::IsButtonPressed(unsigned int KeyScanCode)
{
	if (mJoystickCurrent[KeyScanCode])
	{
		//mKeyTime[KeyScanCode]++;
		return mJoystickCurrent[KeyScanCode];
	}

	return false;
}

int InputManager::IsButtonReleased(unsigned int KeyScanCode)
{
	if (!mJoystickCurrent[KeyScanCode] && mJoystickPrevious[KeyScanCode])
	{
		mKeyTime[KeyScanCode] = 0;
		return true;
	}
	return false;
}

int InputManager::IsButtonTriggered(unsigned int KeyScanCode)
{
	if (mCurrentState[KeyScanCode] && !mPreviousState[KeyScanCode])
	{
		mKeyTime[KeyScanCode] = 0;
		return true;
	}
	if (mJoystickCurrent[KeyScanCode] && !mJoystickPrevious[KeyScanCode])
	{
		mJoystickCurrent[KeyScanCode] = 0;
		return 1;
	}
	return false;
}

glm::vec2 InputManager::GetMouseLocation()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	glm::vec2 wSize = engine->GetWindowSize();
	return glm::vec2((float)x*(ENGINE_WIDTH /wSize.x), (wSize.y - y)*(ENGINE_HEIGHT / wSize.y));
}

Uint32 InputManager::GetTime(unsigned int KeyScanCode)
{
	return mKeyTime[KeyScanCode];
}
