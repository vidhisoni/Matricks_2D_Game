/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class Transform : public Component
{
public:
	Transform();
	
	void Update();

	void Serialize(rapidjson::Value::Object obj);
	glm::mat4 GetModel();
	glm::mat4 GetDebugModel();
	void HandleEvent(Event *pEvent);
	void DeSerialize(std::ofstream* file);

	void CopyTransform(Transform* pTr);

	Component* Create();

	void Clear();

public:
	glm::vec2 mPosition;
	float mZCoord;
	glm::vec2 mScale;
	float mAngle;

};