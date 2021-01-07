/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
Modified By: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"

#include "SDL_keyboard.h"
#include "SDL_mouse.h"

extern Engine* engine;
extern int currentLevel;


Button::~Button()
{

}

void Button::Update()
{
	if (selected) 
		mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("selectedbutton"));
	
	if (!GameObjectManager::GetInstance()->ObjectDetials) {
		glm::vec2 mouseLoc = InputManager::GetInstance()->GetMouseLocation();
		Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		if (COLLISION_MANAGER->PointToRect(mouseLoc, pTr->mPosition, pTr->mScale) || mButtonPressed)
		{
			Audio* audio = static_cast<Audio*>(mpOwner->GetComponent(AUDIO));
			if(engine->IsCursorVisible() && (!muteButtonBGM && !muteButtonSFX))
				mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("selectedbutton"));
			soundOnce++;
			if (soundOnce == 1)
				audio->Play(1);
			if (InputManager::GetInstance()->IsMouseClicked(SDL_BUTTON_LEFT) || mButtonPressed)
			{
				audio->Play(0);
				Text* pTx = static_cast<Text*>(mpOwner->GetComponent(TEXT));
				std::string button_name = pTx->GetText();
				if (button_name == "Start")
				{
					GAMESTATE_MANAGER->ChangeLevel(STORYLINE1);
				}
				else if (button_name == "How To Play")
				{
					GAMESTATE_MANAGER->ChangeInGameMenu(HOWTOPLAY);
				}
				else if (button_name == "Options")
				{
					GAMESTATE_MANAGER->ChangeInGameMenu(SETTINGS);
				}
				else if (button_name == "BACK")
				{	
					if (GAMESTATE_MANAGER->GetCurrentLevel() == LEVELLIST)
					{
						GAMESTATE_MANAGER->ChangeLevel(MAINMENU);
					}
					else
						GAMEOBJECT_MANAGER->RemoveInGameMenu();	
				}
				else if (button_name == "Resume Game" || button_name == "Resume")
				{
					engine->UnPause();
				}
				else if (button_name == "Restart Level")
				{
 					GAMESTATE_MANAGER->ChangeLevel(GAMESTATE_MANAGER->GetCurrentLevel());
 					engine->UnPause();
					
				}
				else if (button_name == "SKIP")
				{
					engine->UnPause();
					GAMESTATE_MANAGER->ChangeLevel(MAINMENU);

				}
				else if (button_name == "MainMenu")
				{
					engine->UnPause();
					GAMESTATE_MANAGER->ChangeLevel(MAINMENU);	
				}
				else if (button_name == "Quit" || button_name == "Quit Game")
				{
					GAMESTATE_MANAGER->ChangeInGameMenu(QUITCONFIRM);
				}
				else if (button_name == "Yes")
				{
					engine->StopEngine();
				}
				else if (button_name == "No")
				{
					GAMEOBJECT_MANAGER->RemoveInGameMenu();
				}
				else if (button_name == "Credits")
				{
					GAMESTATE_MANAGER->ChangeLevel(CREDITS);
				}
				else if (button_name == "Levels")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVELLIST);
				}
				else if (button_name == "Level 1")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL1);
				}
				else if (button_name == "Level 2")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL2);
				}
				else if (button_name == "Level 3")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL3);
				}
				else if (button_name == "Level 4")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL4);
				}
				else if (button_name == "Level 5")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL5);
				}
				else if (button_name == "Level 6")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL6);
				}
				else if (button_name == "Level 7")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL7);
				}
				else if (button_name == "Level 8")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL8);
				}
				else if (button_name == "Level 9")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL9);
				}
				else if (button_name == "Level 10")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL10);
				}
				else if (button_name == "Level 11")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL11);
				}
				else if (button_name == "Level 12")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL12);
				}
				else if (button_name == "Level 13")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL13);
				}
				else if (button_name == "Level 14")
				{
					GAMESTATE_MANAGER->ChangeLevel(LEVEL14);
				}
				else if (button_name == "")
				{
					if (bgm)
					{
						if (AudioManager::GetInstance()->muteBGM) {
							mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("AudioUnMute"));
							bool temp = AudioManager::GetInstance()->muteBGM;
							AudioManager::GetInstance()->muteBGM = !temp;
						}
						else {
							mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("AudioMute"));
							bool temp = AudioManager::GetInstance()->muteBGM;
							AudioManager::GetInstance()->muteBGM = !temp;
						}
					}
					else
					{
						if (AudioManager::GetInstance()->muteSFX) {
							mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("AudioUnMute"));
							bool temp = AudioManager::GetInstance()->muteSFX;
							AudioManager::GetInstance()->muteSFX = !temp;
						}
						else {
							mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("AudioMute"));
							bool temp = AudioManager::GetInstance()->muteSFX;
							AudioManager::GetInstance()->muteSFX = !temp;
						}
					}
				}

				//TODO
				//Resolution Change
				if (button_name == "Small Window") 
				{
					std::cout << "Resolution changed to --> 1024 X 576" << std::endl;
					glm::vec2 temp;
					temp.x = 1024; temp.y = 576;
					engine->ChangetoWindowed();
					engine->ChangeResolution(temp);
					GameObjectManager::GetInstance()->screenRes.x = 1024;
					GameObjectManager::GetInstance()->screenRes.y = 576;
					changeState();

				}
				if (button_name == "Medium Window")
				{
					std::cout << "Resolution changed to --> 1366 X 768" << std::endl;
					glm::vec2 temp;
					temp.x = 1366; temp.y = 768;
					engine->ChangetoWindowed();
					engine->ChangeResolution(temp);
					GameObjectManager::GetInstance()->screenRes.x = 1366;
					GameObjectManager::GetInstance()->screenRes.y = 768;
					changeState();
				}
				if (button_name == "Fullscreen")
				{
					std::cout << "Resolution changed to --> 1920 X 1080" << std::endl;
					engine->ChangetoFullScreen();
					GameObjectManager::GetInstance()->screenRes.x = 1920;
					GameObjectManager::GetInstance()->screenRes.y = 1080;
					changeState();
				}

				//Volume Change
				if (button_name == "-")
				{
					if (bgm) 
					{
						temp = GameObjectManager::GetInstance()->GetValue();
						Text* pTx = static_cast<Text*>(temp[0]->GetComponent(TEXT));
						AudioManager::GetInstance()->Audio_Volume--;
						int temp = (int)AudioManager::GetInstance()->Audio_Volume;
						AudioManager::GetInstance()->SetSongsVolume(temp);
						pTx->SetText(std::to_string(temp));
						//std::cout << "Volume reduced" << std::endl;
					}
					else
					{
						temp = GameObjectManager::GetInstance()->GetValue();
						Text* pTx = static_cast<Text*>(temp[1]->GetComponent(TEXT));
						AudioManager::GetInstance()->SFX_Volume--;
						int temp = (int)AudioManager::GetInstance()->SFX_Volume;
						AudioManager::GetInstance()->SetSFXsVolume(temp);
						pTx->SetText(std::to_string(temp));
						//std::cout << "SFX reduced" << std::endl;
					}
				}
				if (button_name == "+")
				{
					if (bgm) 
					{
						temp = GameObjectManager::GetInstance()->GetValue();
						Text* pTx = static_cast<Text*>(temp[0]->GetComponent(TEXT));
						AudioManager::GetInstance()->Audio_Volume++;
						int temp = (int)AudioManager::GetInstance()->Audio_Volume;
						AudioManager::GetInstance()->SetSongsVolume(temp);
						pTx->SetText(std::to_string(temp));
						//std::cout << "Volume increased" << std::endl;
					}
					else
					{
						temp = GameObjectManager::GetInstance()->GetValue();
						Text* pTx = static_cast<Text*>(temp[1]->GetComponent(TEXT));
						AudioManager::GetInstance()->SFX_Volume++;
						int temp = (int)AudioManager::GetInstance()->SFX_Volume;
						AudioManager::GetInstance()->SetSFXsVolume(temp);
						pTx->SetText(std::to_string(temp));
						//std::cout << "SFX increased" << std::endl;
					}
				}
			}
		}
		else {

			if (selected) {}
			//std::cout << "don't" << std::endl;
			else if (engine->IsCursorVisible()) {
				if(!muteButtonBGM && ! muteButtonSFX)
					mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("transparent"));
			}
			
		soundOnce = 0;
		}
	}
	if (mButtonPressed) {
		mButtonPressed = false;
		soundOnce = 0;
	}
}


void Button::Serialize(rapidjson::Value::Object obj)
{
	mSprite = static_cast<Sprite*>(mpOwner->GetComponent(SPRITE));

	if (obj.HasMember("type1")) type1 = obj["type1"].GetBool();
	if (obj.HasMember("type2")) type2 = obj["type2"].GetBool();
	if (obj.HasMember("type3")) type3 = obj["type3"].GetBool();
	if (obj.HasMember("bgm")) bgm = obj["bgm"].GetBool();
	if (obj.HasMember("Dont")) DontChangeTexture = obj["Dont"].GetBool();
	if (obj.HasMember("MuteButtonSFX")) muteButtonSFX = obj["MuteButtonSFX"].GetBool();
	if (obj.HasMember("MuteButtonBGM")) muteButtonBGM = obj["MuteButtonBGM"].GetBool();

}

void Button::DeSerialize(std::ofstream* file)
{
	*file << "{\n\"Type\":\"Button\"\n},\n";
}

void Button::HandleEvent(Event* pEvent)
{
}

void Button::SelectButton()
{
	mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("selectedbutton"));
}

void Button::DeselectButton()
{
	mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("transparent"));
}

void Button::Clear()
{
	mSprite = nullptr;
	mButtonPressed = false;
	bgm = selected = DontChangeTexture = false;
	type1 = type2 = type3 = false;
	muteButtonBGM = muteButtonSFX = false;
	temp.clear();
	count = 0;
}

Component* Button::Create()
{
	return new Button();
}

void Button::changeState()
{
	this->selected = true;
	for (unsigned int i = 0; i < GameObjectManager::GetInstance()->mGameObjects.size(); ++i)
	{
		if (GameObjectManager::GetInstance()->mGameObjects[i]->GetType() == BUTTONOBJ 
			&& GameObjectManager::GetInstance()->mGameObjects[i] != mpOwner) {
			Button* bton = static_cast<Button*>(GameObjectManager::GetInstance()->mGameObjects[i]->GetComponent(BUTTON));
				bton->selected = false;
		}
	}
}

