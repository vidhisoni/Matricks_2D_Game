/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#include "stdafx.h"

FrameRateController* FrameRateController::pObject = nullptr;

void FrameRateController::SetMaxFrameRate(Uint32 MaxFramerate)
{
	mMaxFramerate = MaxFramerate;
	if (mMaxFramerate != 0)
		mNeededTicksPerFrame = 1000 / mMaxFramerate;
	else
		mNeededTicksPerFrame = 0;
}

FrameRateController* FrameRateController::CreateInstance()
{
	pObject = new FrameRateController();
	return pObject;
}

void FrameRateController::Init()
{
	mTickStart = mTickEnd = mFrameTime = 0;
}


void FrameRateController::Close()
{
	delete pObject;
}

void FrameRateController::FrameStart()
{
	mTickStart = SDL_GetTicks();
}

void FrameRateController::FrameEnd()
{
	mTickEnd = SDL_GetTicks();
	while ((mTickEnd - mTickStart) < mNeededTicksPerFrame)
	{
		mTickEnd = SDL_GetTicks();
	}
	mFrameTime = mTickEnd - mTickStart;
}