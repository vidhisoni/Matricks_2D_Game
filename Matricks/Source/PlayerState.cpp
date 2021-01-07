/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
Modified by: Shantanu Chauhan, Sairaj Padghe, Vidhi Soni
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "SDL_keyboard.h"

PlayerState::PlayerState() : Component(PLAYERSTATE)
{
	mCurrentState = IDLERIGHT;
}

void PlayerState::Update()
{
	if (mpOwner->isPaused)
		return;
	Lua_Update(mCurrentState);
	Body *pBody = static_cast<Body *>(mpOwner->GetComponent(BODY));
	Animator *pAnim = static_cast<Animator *>(mpOwner->GetComponent(ANIMATOR));
	//TODO: Remove Later
	switch (mCurrentState)
	{
	case IDLELEFT:
		if (pAnim)
			pAnim->SetState(animeState);
		pBody->rReturnVelocity().x = 0.0f;
		break;
	case IDLERIGHT:
		if (pAnim)
			pAnim->SetState(animeState);
		pBody->rReturnVelocity().x = 0.0f;
		break;
	case MOVELEFT:
		pBody->ApplyForceX(forceX);
		//pBody->goingLeft = gLeft;
		pBody->rRetrunAction()[GoingLeft] = gLeft;
		//pBody->goingRight = gRight;
		pBody->rRetrunAction()[GoingRight] = gRight;
		if (pAnim && !Pushing)
			pAnim->SetState(animeState);

		break;
	case MOVERIGHT:
		pBody->ApplyForceX(forceX);
		//pBody->goingRight = gRight;
		//pBody->goingLeft = gLeft;
		pBody->rRetrunAction()[GoingRight] = gRight;
		pBody->rRetrunAction()[GoingLeft] = gLeft;
		if (pAnim && !Pushing)
			pAnim->SetState(animeState);
		break;
	case JUMP:
		//THIS IS FOR VARIABLE JUMP
		/*if (mTime < 10 && pBody->rReturnJumpC()<2)
			pBody->ApplyForceY(15000.0f*mTime);
		mCurrentState = mPrevState;*/
		//THIS IS FOR VARIABLE JUMP
		if (mPrevState == MOVELEFT || mPrevState == IDLELEFT)
			pAnim->PlayAnimation("jumpleft",NOEVENT,0.7f);
		if (mPrevState == MOVERIGHT || mPrevState == IDLERIGHT)
			pAnim->PlayAnimation("jumpright",NOEVENT,0.7f);
		pBody->ApplyForceY(forceY);
		pBody->rRetrunAction()[Jumping] = true;
		pBody->rRetrunAction()[InAir] = true;
		//pBody->rRetrunAction()[OnGround] = false;
		mCurrentState = mPrevState;
		break;
	case DAB:
		pAnim->PlayAnimation("dab",NOEVENT,1.5f);
		pAnim->SetPrevState("dab");
		break;
	}
}

void PlayerState::DeSerialize(std::ofstream* file)
{
	*file << "\"PlayerState\":{";
	*file << "\"Tools\":[" << (mtool[0]?"true":"false") << "," <<(mtool[1]?"true":"false")<< "]";
	*file << "},";
	//*file << "\"Accel\":[" << mAccel.x << "," << mAccel.y << "],";
	//*file << "\"Mass\":" << mMass << ",";
	//*file << "\"AABB\":[" << mScaleFactor.x << "," << mScaleFactor.y << "],";

}

void PlayerState::HandleEvent(Event *pEvent)
{
	if (pEvent->mType == ONGROUNDCOLLISION)
	{
		RENDER_MANAGER->mCamera.SnaptoY();
	}
	if (pEvent->mType == CHARGEPICKUP)
	{
		mtool[CHARGED] = true;
		Transform *pTr = static_cast<Transform*>(mpOwner->mChildObjects[2]->GetComponent(TRANSFORM));
		pTr->mScale = glm::vec2(100.0f);
	}
	if (pEvent->mType == NUMBERBOXCOLLISION)
	{
		if (static_cast<Body*>(pEvent->pObject->GetComponent(BODY))->ReturnMass() != 0.0f)
		{
			Animator *pAnim = static_cast<Animator *>(mpOwner->GetComponent(ANIMATOR));
			if (pEvent->mContact->colSide == LEFT && INPUT_MANAGER->IsPressed(SDL_SCANCODE_LEFT))
			{
				pAnim->SetState("pushleft");
				Pushing = true;
			}
			else if (pEvent->mContact->colSide == RIGHT && INPUT_MANAGER->IsPressed(SDL_SCANCODE_RIGHT))
			{
				pAnim->SetState("pushright");
				Pushing = true;
			}
			else
			{
				Pushing = false;
			}
		}
	}
	if (pEvent->mType == NUMBERBOXTRIGGER)// && InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_E))
	{	
		if (INPUT_MANAGER->IsTriggered(SDL_SCANCODE_F) && mtool[CHARGED] && !static_cast<Text*>(pEvent->pObject->mParentObject->GetComponent(TEXT))->IsActive())
		{
			Event *Chargeused = new Event(CHARGEUSED, pEvent->pObject);
			pEvent->pObject->mParentObject->HandleEvent(Chargeused);
			delete Chargeused;
			mtool[CHARGED] = false;
			Transform *pTr = static_cast<Transform*>(mpOwner->mChildObjects[2]->GetComponent(TRANSFORM));
			pTr->mScale = glm::vec2(0.0f);
		}
		else
		{
			Event *Changenum = new Event(CHANGENUMBER, pEvent->pObject);
			Changenum->mTimer = 0.0f;
			EVENT_MANAGER->AddTimedEvent(Changenum);
			Audio* audio = static_cast<Audio *>(mpOwner->GetComponent(AUDIO));
			//audio->Play(1);
		}
	}

	if (pEvent->mType == LEVELCOMPLETE)
	{
		std::string pTx = static_cast<Text*>(mpOwner->mChildObjects[0]->GetComponent(TEXT))->GetText();
		std::string pDoor = static_cast<Text*>(pEvent->pObject->GetComponent(TEXT))->GetText();
		int Cval = 0, Dval = 0;
		if (pTx != "")
			Cval = std::stoi(pTx);
		if(pDoor != "")
			Dval = std::stoi(pDoor);
		if(Cval == Dval)
			ChangeState(DAB);
	}
}

Component* PlayerState::Create()
{
	return new PlayerState();
}

void PlayerState::ChangeState(PlayerStates state,int _time)
{
	if (mCurrentState != state)
	{
		Pushing = false;
		mPrevState = mCurrentState;
		mCurrentState = state;
		mTime = _time;
	}
}

void PlayerState::Clear()
{
	mCurrentState = mPrevState = IDLERIGHT;
	mtool[PLUS] = false;
	mtool[MINUS] = false;
	mtool[CHARGED] = false;
	Pushing = false;
	forceX = forceY = 0.0f;
	gLeft = gRight = false;
	animeState = "";
	mTime = 0;
	Pushing = false;
}

void PlayerState::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("Tools")) {
		rapidjson::Value::Array Tools = obj["Tools"].GetArray();
		mtool[PLUS]= Tools[0].GetBool();
		mtool[MINUS] = Tools[1].GetBool();
		mtool[CHARGED] = false;
	}
	if (obj.HasMember("CurrentState")) {
		int curr= obj["CurrentState"].GetInt();
		if (curr == 0)
			mCurrentState = IDLELEFT;
		else
			mCurrentState = IDLERIGHT;
	}
	Pushing = false;
}


//============= LUA FUNCTIONS ======================================

void PlayerState::Lua_Update(int x)
{
	lua_State* L = luaL_newstate();
	int gL, gR;

	std::string t = "test.lua";
	std::string temp = "Resources\\Scripts\\" + t;


// 	if (luaL_loadfile(L, temp.c_str()) || lua_pcall(L, 0, 0, 0)) {
// 		std::cout << "Error: Failed to load lua file" << std::endl;
// 		return ;
// 	}

	int initError = luaL_loadfile(L, temp.c_str());
	switch (initError)
	{
	case 0:
		// The file loaded okay, so call it as a protected function - to stop fatal errors from exiting the program
		lua_pcall(L, 0, 0, 0);
		break;
	case LUA_ERRFILE:
		std::cerr << "Cannot find / open lua script file: " << std::endl << "Skipping Lua init." << std::endl;
		break;
	case LUA_ERRSYNTAX:
		std::cerr << "Syntax error during pre-compilation of script file: "  << std::endl << "Skipping Lua init." << std::endl;
		break;
	case LUA_ERRMEM:
		// Treating this as fatal, since it means other Lua calls are unlikely to work either
		std::cerr << "Fatal memory allocation error during processing of script file: "  << std::endl;
		break;
	}

	lua_getglobal(L, "update");

	if (lua_isfunction(L, -1))
	{
	//	std::cout << "Function loaded onto stack successfully...!!" << std::endl;

		lua_pushnumber(L, x);
		lua_pcall(L, 1, 1, 0);

		if (!lua_isnil(L, -1)) {

			forceX = getFloatField(L, "forceX");
			forceY = getFloatField(L, "forceY");
			animeState = getStringField(L, "animeState");
			gL = getIntField(L, "gLeft");
			gR = getIntField(L, "gRight");

		}
		gLeft = (gL == 1) ? true : false;
		gRight = (gR == 1) ? true : false;
	}

	lua_pop(L, 1);
	lua_close(L);

}

float PlayerState::getFloatField(lua_State* L, const char* key)
{
	lua_pushstring(L, key);
	lua_gettable(L, -2);  // get table[key]

	float result = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);  // remove number from stack
	return result;
}

int PlayerState::getIntField(lua_State * L, const char * key)
{

	lua_pushstring(L, key);
	lua_gettable(L, -2);  // get table[key]

	int result = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);  // remove number from stack
	return result;
}

std::string PlayerState::getStringField(lua_State * L, const char * key)
{
	lua_pushstring(L, key);
	lua_gettable(L, -2);  // get table[key]

	std::string result = lua_tostring(L, -1);
	lua_pop(L, 1);  // remove string from stack
	return result;
}