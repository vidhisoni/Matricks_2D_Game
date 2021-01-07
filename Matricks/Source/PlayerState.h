/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
Modified by: Shantanu Chauhan, Sairaj Padghe, Vidhi Soni
- End Header --------------------------------------------------------*/
#pragma once
#pragma comment (lib, "lua5.1.lib")

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

using namespace luabridge;
enum TOOL
{
	PLUS,
	MINUS,
	CHARGED
};

enum PlayerStates
{
	IDLELEFT,
	IDLERIGHT,
	MOVELEFT,
	MOVERIGHT,
	JUMP,
	DAB,
	INTERACT
};

class PlayerState : public Component
{
public:
	PlayerState();

	void Update();
	void DeSerialize(std::ofstream* file);
	void HandleEvent(Event *pEvent);
	Component* Create();
	void ChangeState(PlayerStates state,int _time = 0);
	inline PlayerStates GetCurrentState() { return mCurrentState; }
	void Clear();
	void Serialize(rapidjson::Value::Object obj);
	// TODO : temporary function.. will move them to lua class later 
	void Lua_Update(int x);
	float getFloatField(lua_State* L, const char* key);
	int getIntField(lua_State* L, const char* key);
	std::string getStringField(lua_State* L, const char* key);

	float forceX, forceY;
	bool gLeft, gRight;
	std::string animeState;
	bool mtool[3];
private:
	int mTime;
	PlayerStates mCurrentState, mPrevState;
	bool Pushing;
};