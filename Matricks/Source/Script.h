/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author:  Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once

class Script : public Component
{
public:
	Script();

	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);

	void Clear();
	Component* Create();

	void HandleEvent(Event* pEvent);

	std::string GetScript();


	//Script files
	void setPositionLua(const std::string& aa);
	void newLuaPos(float x, float y, float z);
	void UpDown();
	void LeftRight();
	//void setUpDown(bool setMove,int timer);
	void setUpDown(bool setMove);

	void setLeftRight(bool setMove/*, int timer*/);

	//TODO Temp rotation
	void RotateObj();
	void setRotation(bool setMove);

	int mTimerLimit;
	float leftLimit, rightLimit;
	float upLimit, downLimit;
	float speed;
private:
	std::string ScriptName;

	bool  mGoingUp, flagUP, flagLEFT, rotat, mGoingLeft;

	bool moveUp, moveDown;
	bool moveLeft, moveRight;
	

	Transform *pTr;
	Body *pBody;

};