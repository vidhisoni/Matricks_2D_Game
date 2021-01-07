/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
Modified By: Sai Sindhu Jannali
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "key.h"
#include "EventManager.h"

key::key():Component(KEY)
{
	doorLock = false;
}

void key::Update()
{

}

void key::Clear()
{

}

Component* key::Create()
{
	return new key();
}

void key::HandleEvent(Event* pEvent)
{

}

