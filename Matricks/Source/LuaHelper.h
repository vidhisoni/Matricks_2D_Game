#pragma once
#pragma comment (lib, "lua5.1.lib")

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class LuaHelper : public Component
{
public:
	LuaHelper();
	void Update();
	void DeSerialize(std::ofstream* file);

	void Clear();
	Component* Create();

	void HandleEvent(Event* pEvent);

public:
	void setPositionLua(const std::string& aa);
	void newLuaPos(float x, float y, float z);
	void setUpDown(bool setMove/*,int timer*/);

};