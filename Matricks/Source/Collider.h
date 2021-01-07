/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once
class Body;
class Collider : public Component
{
public:
	Collider();
	~Collider();

	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);
	void HandleEvent(Event* pEvent);
	void Clear();
	void Initialize();
	Component* Create();
private:
	Body* pBody;
	Shape* mpShape;
	glm::vec2 mPosition;
	glm::vec2 mScaleFactor;
	glm::vec2 mScale;
};