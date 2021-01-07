/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class Story : public Component
{
public:
	Story();
	~Story();

	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void HandleEvent(Event* pEvent);
	void Clear();
	Component* Create();

private:
	bool mDone;
	bool isLastFrame;
	int mFrameNum;
	bool mStart;
	bool isSound;
	float mTime;
};