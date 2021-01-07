/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class Subsciber : public Component
{
public:
	Subsciber();

	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);

	void Clear();
	Component* Create();

	void HandleEvent(Event* pEvent);

	void SubscribeToEvent();

public:
	std::vector<std::string> mEventsSubscribed;
};