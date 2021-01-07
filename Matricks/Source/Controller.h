/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
Modified by: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once

class Controller : public Component
{
public:
	Controller();

	void Update();
	void DeSerialize(std::ofstream* file);

	void Clear();
	Component* Create();

	void HandleEvent(Event* pEvent);
	int currentLevel;
	bool AudioDebug;
	int test = 1;
	bool isPromptAudio = false;
};