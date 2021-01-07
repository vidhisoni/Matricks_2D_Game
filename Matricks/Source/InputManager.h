/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
Modified by: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once

#include "SDL_stdinc.h"
#include"SDL/include/SDL_gamecontroller.h"
enum controller//DONT LIKE THIS TODO: CHANGE
{
	NUMBER = 15
};
class InputManager
{
	static InputManager* pObject;
	InputManager() {}

public:

	static InputManager* CreateInstance();
	inline static InputManager* GetInstance() { return pObject; }

	void Init();
	void Close();
	void Update();


	bool IsPressed(unsigned int KeyScanCode); //From SDL_Scancode
	bool IsTriggered(unsigned int KeyScanCode);
	bool IsReleased(unsigned int KeyScanCode);

	bool IsMouseClicked(unsigned int KeyScanCode);
	int IsButtonPressed(unsigned int KeyScanCode);
	int IsButtonReleased(unsigned int KeyScanCode);
	int IsButtonTriggered(unsigned int KeyScanCode);

	glm::vec2 GetMouseLocation();
	Uint32 GetTime(unsigned int KeyScanCode);
private:
	Uint8 mCurrentState[512];
	Uint32 mKeyTime[512];
	Uint8 mPreviousState[512];
	bool mPrevMouseState[3];
	bool mCurrentMouseState[3];
	SDL_GameController *mGameController;
	int mJoystickCurrent[NUMBER];
	int mJoystickPrevious[NUMBER];
	bool mControllerActive;
};

