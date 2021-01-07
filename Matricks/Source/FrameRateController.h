/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once
#include<SDL_timer.h>

class FrameRateController
{
private:
	Uint32 mMaxFramerate;
	Uint32 mTickStart;
	Uint32 mTickEnd;
	Uint32 mFrameTime;
	Uint32 mNeededTicksPerFrame;

private:
	static FrameRateController* pObject;
	FrameRateController() {};

public:
	void SetMaxFrameRate(Uint32 MaxFramerate);
	static FrameRateController* CreateInstance();
	inline static FrameRateController* GetInstance() { return pObject; }
	void Init();
	void Close();
	void FrameStart();
	void FrameEnd();
	inline Uint32 GetFrameTime() { return mFrameTime; }
};
