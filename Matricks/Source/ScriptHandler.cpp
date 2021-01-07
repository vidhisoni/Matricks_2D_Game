/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"

using namespace luabridge;

ScriptHandler* ScriptHandler::pObject = nullptr;


ScriptHandler * ScriptHandler::CreateInstance()
{
 	pObject = new ScriptHandler();
 	return pObject;
}

void ScriptHandler::RegisterLuaFunction()
{
	
}

void ScriptHandler::Init()
{
	//RegisterLuaFunction();
}

void ScriptHandler::Close()
{
	delete pObject;
}

void ScriptHandler::Update()
{
	

	for (auto go : GameObjectManager::GetInstance()->mGameObjects)
	{
		//std::cout << go->GetType() << std::endl;
		Script *pScript = static_cast<Script*>(go->GetComponent(SCRIPT));

		if (pScript != nullptr) {


			lua_State* L = luaL_newstate();
			luaL_openlibs(L);

			

			std::string temp = "Resources/Scripts/" + pScript->GetScript();

// 			if (luaL_loadfile(L, temp.c_str())) {
// 				std::cout << "Error: Failed to load lua file" << std::endl;
// 				return;
// 			}

			int initError = luaL_loadfile(L, temp.c_str());
			switch (initError)
			{
			case 0:
				// The file loaded okay, so call it as a protected function - to stop fatal errors from exiting the program
				//std::cout << "Scripts in LuaHandler loaded successfully" << std::endl;
				break;
			case LUA_ERRFILE:
				//std::cerr << "Cannot find / open lua script file: " << std::endl << "Skipping Lua init." << std::endl;
				break;
			case LUA_ERRSYNTAX:
				//std::cerr << "Syntax error during pre-compilation of script file: " << std::endl << "Skipping Lua init." << std::endl;
				break;
			case LUA_ERRMEM:
				// Treating this as fatal, since it means other Lua calls are unlikely to work either
				//std::cerr << "Fatal memory allocation error during processing of script file: " << std::endl;
				break;
			}


// 			luabridge::getGlobalNamespace(L)
// 				.beginClass<LuaHelper>("LuaHelper")
// 				.addFunction("setPositionLua", &LuaHelper::setPositionLua)
// 				.addFunction("newLuaPos", &LuaHelper::newLuaPos)
// 				.addFunction("setUpDown", &LuaHelper::setUpDown)
// 				.endClass();

			luabridge::getGlobalNamespace(L)
				.beginClass<Script>("Script")
				.addFunction("setPositionLua", &Script::setPositionLua)
				.addFunction("newLuaPos", &Script::newLuaPos)
				.addFunction("setUpDown", &Script::setUpDown)
				.addFunction("setRotation", &Script::setRotation)
				.addFunction("setLeftRight", &Script::setLeftRight)
				.endClass();



			Script *luaObject = static_cast<Script *>(go->GetComponent(SCRIPT));
			//LuaHelper *luaObject = static_cast<LuaHelper *>(go->GetComponent(LUAHELPER));
			luabridge::push(L, luaObject);
			lua_setglobal(L, "luaObject");



// 			if (luaL_dofile(L, "abc.lua") != 0) {
// 			//	std::cout << "WORKING" << std::endl;
// 			}


			lua_pcall(L, 0, 0, 0);
			//lua_pop(L, 1);
			//std::cout << "Script name = " << pScript->GetScript() << std::endl;
// 				std::string result = lua_tostring(L, 1);
// 				std::cout << result << std::endl;
			lua_close(L);
		

		}
	}
}	

void ScriptHandler::LuaClose()
{

}
