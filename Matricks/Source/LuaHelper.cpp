#include "stdafx.h"

LuaHelper::LuaHelper() : Component(LUAHELPER)
{
}

void LuaHelper::Update()
{

}

void LuaHelper::DeSerialize(std::ofstream* file)
{

}

void LuaHelper::Clear()
{

}

Component* LuaHelper::Create()
{
	return new LuaHelper();
}

void LuaHelper::HandleEvent(Event* pEvent)
{

}

void LuaHelper::setPositionLua(const std::string& aa)
{
// 	Transform *trans = static_cast<Transform *>(mpOwner->GetComponent(TRANSFORM));
// 
// 	trans->setPositionLua(aa);

}

void LuaHelper::newLuaPos(float x, float y, float z)
{
// 	Transform *trans = static_cast<Transform *>(mpOwner->GetComponent(TRANSFORM));
// 
// 	trans->newLuaPos(x, y, z);
}

void LuaHelper::setUpDown(bool setMove/*,int timer*/)
{
// 	std::cout << "SetUpDown call by" << mpOwner->GetType() << std::endl;
// 	Transform *trans = static_cast<Transform *>(mpOwner->GetComponent(TRANSFORM));
// 
// 	trans->setUpDown(setMove/*,timer*/);
}
