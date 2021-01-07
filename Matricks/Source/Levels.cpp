/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"
///
#include <winerror.h> //for HRESULT
#include <comutil.h> //for _bstr_t (used in the string conversion)
#include "shlobj.h"
//

LevelList::LevelList() : Component(LEVEL)
{
}

LevelList::~LevelList()
{

}

void LevelList::Update()
{

}

void LevelList::Serialize(rapidjson::Value::Object obj)
{
	LPWSTR wszPath = NULL;
	HRESULT hr;
	std::string mFilename;
	hr = SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_CREATE, NULL, &wszPath);

	if (SUCCEEDED(hr)) {
		_bstr_t bstrPath(wszPath);
		mFilename = std::string((char*)bstrPath) + "\\Matricks\\CompletedLevels.json";
	}

	std::ifstream file(mFilename);
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	rapidjson::Document root;
	root.Parse(contents.c_str());

	rapidjson::Value::Array levels = root["Levels"].GetArray();

	if (levels.Size() == 0)
	{
		GameObject* pButton = GAMEOBJECT_FACTORY->LoadObject("text.json");
	}

	for (unsigned int i = 0; i < levels.Size(); i+=2)
	{
		GameObject* pButton = GAMEOBJECT_FACTORY->LoadObject("button.json");
		Transform* pTr = static_cast<Transform*>(pButton->GetComponent(TRANSFORM));
		pTr->mPosition = glm::vec2(810, 750 - i * 50);
		pTr->mScale = glm::vec2(270.0f, 50.0f);
		static_cast<Sprite*>(pButton->GetComponent(SPRITE))->SetZCoord(0.1f);
		static_cast<Body*>(pButton->GetComponent(BODY))->Initialize();
		std::string str;
		unsigned int val = levels[i].GetUint();
		str = "Level " + std::to_string(val);
		Text* pTx = static_cast<Text*>(pButton->GetComponent(TEXT));
		pTx->SetText(str);
		pTx->SetColor(glm::vec3(0.0f));
		pTx->mOffset = glm::vec2(-0.2f, -0.2f);
	}
	for (unsigned int i = 1; i < levels.Size(); i+=2)
	{
		GameObject* pButton = GAMEOBJECT_FACTORY->LoadObject("button.json");
		Transform* pTr = static_cast<Transform*>(pButton->GetComponent(TRANSFORM));
		pTr->mPosition = glm::vec2(1110, 750 - (i-1) * 50);
		pTr->mScale = glm::vec2(270.0f, 50.0f);
		static_cast<Sprite*>(pButton->GetComponent(SPRITE))->SetZCoord(0.1f);
		static_cast<Body*>(pButton->GetComponent(BODY))->Initialize();
		std::string str;
		unsigned int val = levels[i].GetUint();
		str = "Level " + std::to_string(val);
		Text* pTx = static_cast<Text*>(pButton->GetComponent(TEXT));
		pTx->SetText(str);
		pTx->SetColor(glm::vec3(0.0f));
		pTx->mOffset = glm::vec2(-0.2f, -0.2f);
	}

}

void LevelList::HandleEvent(Event* pEvent)
{
}

void LevelList::Clear()
{
}


Component* LevelList::Create()
{
	return new LevelList();
}
