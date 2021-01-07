/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
Modified By: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once

class Sprite;

class Button : public Component
{
public:
	Button() :Component(BUTTON), selected(false), type1(false), type2(false) , type3(false),bgm(false),
		DontChangeTexture(false), muteButtonSFX(false), muteButtonBGM(false) {}
	~Button();

	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);
	void HandleEvent(Event* pEvent);
	void SelectButton();
	void DeselectButton();
	inline void PressButton() { mButtonPressed = true; }
	void Clear();
	Component* Create();
	void changeState();
	bool selected;
	bool type1, type2, type3;
	bool bgm;
	bool DontChangeTexture;
	bool muteButtonSFX, muteButtonBGM;

private:
	Sprite* mSprite;
	int count = 0;
	//GameObject* temp = nullptr;
	int soundOnce = 0;
	std::vector<GameObject *> temp;
	bool mButtonPressed;
};