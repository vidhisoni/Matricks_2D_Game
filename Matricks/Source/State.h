/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once
enum NPCStates
{
	ACTIVE,
	CLOSED,
	OPENING,
	OPENED
};

class State :public Component
{
public:
	State();

	void Update();

	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);
	inline void SetState(NPCStates state) { mCurrentState = state; }
	Component* Create();
	void Clear();
	
private:
	NPCStates mCurrentState;
};