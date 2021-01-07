/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class GameObject;
class Texture;
class Event;


class ParallaxBg : public Component
{
public:
	ParallaxBg();
	~ParallaxBg();

	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);
	void HandleEvent(Event* pEvent);
	Component* Create();
	void Clear();

private:
	std::vector<std::pair<GameObject*,float>> mBackgrounds;
	glm::vec2 LastCamPosition;
};