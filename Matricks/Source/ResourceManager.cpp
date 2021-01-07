/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"

ResourceManager* ResourceManager::pObject = nullptr;

ResourceManager* ResourceManager::CreateInstance()
{
	pObject = new ResourceManager();
	return pObject;
}

void ResourceManager::Init()
{
	std::string fullPath = "Resources/Textures/Textures.json";
	std::ifstream file(fullPath);
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	rapidjson::Document doc;
	doc.Parse(contents.c_str());
	
	//Load Textures
	rapidjson::Value::Array textures = doc["Textures"].GetArray();
	std::string TexType;
	for (unsigned int i = 0; i < textures.Size(); ++i)
	{
		rapidjson::Value::Object TexObj = textures[i].GetObject();

		TexType = TexObj["Type"].GetString();;

		std::string pFilePath = "Resources/Textures/";
		pFilePath += TexObj["File"].GetString();

		mTextures[TexType] = new Texture(pFilePath);
	}

	//Load Animations
	rapidjson::Value::Array animations = doc["Animations"].GetArray();
	std::string AnimType;
	for (unsigned int i = 0; i < animations.Size(); ++i)
	{
		rapidjson::Value::Object AnimObj = animations[i].GetObject();

		AnimType = AnimObj["Type"].GetString();

		std::string pFilePath = "Resources/Textures/Animations/";
		pFilePath += AnimObj["FileDir"].GetString();

		std::vector<Texture*> spritesheet;

		for (const auto& dir_entry : std::experimental::filesystem::directory_iterator(pFilePath)) {

			std::string s = dir_entry.path().generic_string();
			Texture *TempTex = new Texture(s);
			spritesheet.push_back(TempTex);
		}
		
		mAnimations[AnimType] = spritesheet;
	}
}

void ResourceManager::Close()
{
	for (auto t : mTextures)
	{
		delete t.second;
	}
	mTextures.clear();

	for (auto a : mAnimations)
	{
		for (auto s : a.second)
		{
			delete s;
		}
		a.second.clear();
	}
	mAnimations.clear();

	delete pObject;
}

Texture* ResourceManager::GetTexture(std::string TexType)
{
	if (mTextures.find(TexType) != mTextures.end())
		return mTextures[TexType];
	else
		LogError(TexType << "Texture Not Found");
}

std::vector<Texture*> ResourceManager::GetAnimation(std::string AnimType)
{
	return mAnimations[AnimType];
}
