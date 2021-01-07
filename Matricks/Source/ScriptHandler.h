/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once
#pragma comment (lib, "lua5.1.lib")

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class ScriptHandler
{

	static ScriptHandler* pObject;
	ScriptHandler() {}
public:

	void Init();
	void Close();

	static ScriptHandler* CreateInstance();
	inline static ScriptHandler* GetInstance() { return pObject; }

	void RegisterLuaFunction();

	
	void Update();
	void LuaClose();
};