/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class MenuControl : public Component
{
public:
	MenuControl();
	~MenuControl();

	void Update();
	void HandleEvent(Event* pEvent);
	void Serialize(rapidjson::Value::Object obj);
	
	void Clear();
	Component* Create();


private:
	std::vector<Button*> mButtons;
	int mSelectedButton;
};