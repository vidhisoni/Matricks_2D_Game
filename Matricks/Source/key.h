/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
Modified By: Sai Sindhu Jannali
- End Header --------------------------------------------------------*/
#pragma once

#include "Component.h"

class key : public Component
{
public:
	key();

	void Update();

	void Clear();
	Component* Create();

	void HandleEvent(Event* pEvent);
	bool doorLock, coinsReached;
};