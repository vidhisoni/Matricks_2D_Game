/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "SDL_keyboard.h"

extern Engine* engine;

MenuControl::MenuControl() : Component(MENUCONTROL)
{

}

MenuControl::~MenuControl()
{

}

void MenuControl::Update()
{
	if (!mpOwner->mActive)
		return;
	InputManager* pInputMgr = INPUT_MANAGER;
	if (pInputMgr->IsTriggered(SDL_SCANCODE_UP))
	{
		//Move Up
		mButtons[mSelectedButton]->DeselectButton();
		mSelectedButton--;
		if (mSelectedButton < 0)
			mSelectedButton = 0;
		mButtons[mSelectedButton]->SelectButton();
		engine->HideCursor();
	}
	if (pInputMgr->IsTriggered(SDL_SCANCODE_DOWN))
	{
		//Move Down
		mButtons[mSelectedButton]->DeselectButton();
		mSelectedButton++;
		if (mSelectedButton >= mButtons.size())
			mSelectedButton = mButtons.size() - 1;
		mButtons[mSelectedButton]->SelectButton();
		engine->HideCursor();
	}
	if (pInputMgr->IsTriggered(SDL_SCANCODE_RETURN))
	{
		//Select the selected menu
		if(mButtons[mSelectedButton]->mpOwner->mActive)
			mButtons[mSelectedButton]->PressButton();
		engine->HideCursor();
	}
	if (pInputMgr->IsTriggered(SDL_SCANCODE_ESCAPE))
	{
		//Go Back
		engine->HideCursor();
	}

}

void MenuControl::HandleEvent(Event* pEvent)
{

}

void MenuControl::Serialize(rapidjson::Value::Object obj)
{
	for (auto g : GAMEOBJECT_MANAGER->mGameObjects)
	{
		if (g->GetType() == BUTTONOBJ && g->mActive)
			mButtons.push_back(static_cast<Button*>(g->GetComponent(BUTTON)));
	}
	mSelectedButton = 0;
}

void MenuControl::Clear()
{
	mButtons.clear();
	mSelectedButton = 0;
}

Component* MenuControl::Create()
{
	return new MenuControl();
}

