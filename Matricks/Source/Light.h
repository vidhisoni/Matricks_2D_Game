/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class Light : public Component
{
public:
	Light();
	~Light();

	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void HandleEvent(Event* pEvent);
	void DeSerialize(std::ofstream* file);
	void Clear();
	Component* Create();

private:
	LightData mData;
	glm::vec2 mOffset;
	float mDistFactor;
};
