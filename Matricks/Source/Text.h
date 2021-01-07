/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
Modified by: Vidhi Soni
- End Header --------------------------------------------------------*/
#pragma once
class Text : public Component
{
public:
	Text();

	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);

	void Clear();
	Component* Create();

	void HandleEvent(Event* pEvent);

	inline int GetValue() { return std::stoi(mData.Text); }
	inline std::string GetText() { return mData.Text; }
	inline void SetText(std::string text) {	mData.Text = text; }
	inline void SetColor(glm::vec3 color) { mData.Color = color; }
	inline bool IsActive() { return mActive; }
	void MakeInactive();
	void MakeActive();
	glm::vec2 mOffset;

private:
	int mValue;
	TextData mData;
	
	bool mActive;
	//TODO REMOVE
	int score;
	bool isDirty;
};