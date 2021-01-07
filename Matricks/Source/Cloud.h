/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class Transform;

class Cloud : public Component
{
public:
	Cloud();
	~Cloud();

	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);
	void HandleEvent(Event* pEvent);
	inline void SetScale(glm::vec2 scale) { mScale = scale; pTrans->mScale = glm::vec2(0.0f); }

	void Clear();
	Component* Create();


private:
	Transform* pTrans;
	glm::vec2 mOffset;
	glm::vec2 mScale;
	bool mEnable;
	bool mUsed;
};