/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
Modified By: Sai Sindhu Jannali
- End Header --------------------------------------------------------*/
#pragma once

class AlarmComponent : public Component
{
public:
	AlarmComponent();

	void Update();
	void DeSerialize(std::ofstream* file);

	void Clear();
	Component* Create();

	void HandleEvent(Event* pEvent);
	bool moveRight, moveLeft,inOUt;
	bool playAudio;
};