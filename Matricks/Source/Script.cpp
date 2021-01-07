/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified by: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"
Script::Script():Component(SCRIPT)
{
	mGoingUp = false;
	rotat = false;
	mGoingLeft = false;
	moveUp = true; moveDown = false;
	moveLeft = true; moveRight = false;
	
}

void Script::Update()
{
	pTr = static_cast<Transform *>(mpOwner->GetComponent(TRANSFORM));
	pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));
	if (flagUP)
	{
		UpDown();
	}
	if (rotat)
	{
		RotateObj();
	}
	if (flagLEFT)
	{
		LeftRight();
	}

}

void Script::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("scriptName"))
		ScriptName = obj["scriptName"].GetString();

	if (obj.HasMember("LeftLimit")) {
		leftLimit = obj["LeftLimit"].GetFloat();
		downLimit = 0.0f;	
	}
	if (obj.HasMember("RightLimit")) {
		rightLimit = obj["RightLimit"].GetFloat();
		upLimit = 0.0f;
	}
	if (obj.HasMember("UpLimit")) {
		upLimit = obj["UpLimit"].GetFloat();
		rightLimit = 0.0f;
	}
	if (obj.HasMember("DownLimit")) {
		downLimit = obj["DownLimit"].GetFloat();
		leftLimit = 0.0f;
	}
	if (obj.HasMember("Speed")) {
		speed = obj["Speed"].GetFloat();
	}
}

void Script::DeSerialize(std::ofstream* file)
{
//	*file << "\"scriptName\":\"" << ScriptName << "\"\n},\n";
	if (speed != 0) {
		*file << "\"Script\":{";
		*file << "\"UpLimit\":" << upLimit << ",\n";
		*file << "\"DownLimit\":" << downLimit << ",\n";
		*file << "\"Speed\":" << speed << "\n},";
	}
}

void Script::Clear()
{
}

Component* Script::Create()
{
	return new Script();
}

void Script::HandleEvent(Event * pEvent)
{
	if (pEvent->mType == TRAPCOLLISION)
	{
	}
	if (pEvent->mType == OUTOFBOUND)
	{
		GAMESTATE_MANAGER->ChangeLevel(GAMESTATE_MANAGER->GetCurrentLevel());
	}
}

std::string Script::GetScript()
{
	return ScriptName;
}

void Script::setPositionLua(const std::string& aa)
{
	//std::cout << "setPositionLua call by" << mpOwner->GetType() << std::endl;

// 	std::cout << "New Position x = " << x << std::endl;
// 	std::cout << "New Position y = " << y << std::endl;
//	std::cout << "New Position  = " << aa << std::endl;
}

void Script::newLuaPos(float x, float y, float z)
{
	// 	std::cout << "New Position x = " << x << std::endl;
//  	std::cout << "New Position y = " << y << std::endl;
// 	std::cout << "New Position z = " << z  << std::endl;

	Body *pBody = static_cast<Body *>(mpOwner->GetComponent(BODY));

	pTr->mPosition.x = x;
	pTr->mPosition.y = y;
	pBody->Initialize();

	if (mpOwner->GetType() == PLAYER)
	{
		Text* pTx = static_cast<Text*>(mpOwner->mChildObjects[0]->GetComponent(TEXT));
		pTx->SetText(std::string("0"));
	}
}

void Script::UpDown()
{
	if(mpOwner->GetType() != LIFT)
		pTr->mAngle += 2.5f;
	if (moveUp)
	{
		pTr->mPosition.y += speed;
		moveDown = (pTr->mPosition.y > upLimit) ? true : false;
		if (pTr->mPosition.y > upLimit)
			pTr->mPosition.y = upLimit - 10.0f;
	}

	if (moveDown)
	{
		pTr->mPosition.y -= speed;
		moveUp = (pTr->mPosition.y < downLimit) ? true : false;
		if (pTr->mPosition.y < downLimit)
			pTr->mPosition.y = downLimit + 10.0f;
	}
	pBody->Initialize();

}

void Script::LeftRight()
{
	pTr->mAngle += 2.5f;
	if (moveLeft)
	{
		pTr->mPosition.x -= speed;
		moveRight = (pTr->mPosition.x < leftLimit) ? true : false;
		if (pTr->mPosition.x < leftLimit)
			pTr->mPosition.x = leftLimit + 10.0f;
	}

	if (moveRight)
	{
		pTr->mPosition.x += speed;
		moveLeft = (pTr->mPosition.x > rightLimit) ? true : false;
		if (pTr->mPosition.x > rightLimit)
			pTr->mPosition.x = rightLimit - 10.0f;
	}
	pBody->Initialize();
}

void Script::setUpDown(bool setMove)
{
	//	std::cout << "SetUpDown call by" << mpOwner->GetType() << std::endl;
	flagUP = setMove;
	//mTimerLimit = timer;
}

void Script::setLeftRight(bool setMove/*, int timer*/)
{
	flagLEFT = setMove;
	//mTimerLimit = timer;
}

void Script::RotateObj()
{

	pTr->mAngle += 2.5f;
	pBody->Initialize();
}

void Script::setRotation(bool setMove)
{
	rotat = setMove;
}
