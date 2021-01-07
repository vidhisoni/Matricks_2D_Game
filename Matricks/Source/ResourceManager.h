/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

class Texture;

class ResourceManager
{
	static ResourceManager* pObject;
	ResourceManager() { }
public:
	static ResourceManager* CreateInstance();
	inline static ResourceManager* GetInstance() { return pObject; }
	void Init();
	void Close();
	Texture* GetTexture(std::string TexType);
	std::vector<Texture*> GetAnimation(std::string AnimType);

private:
	std::map<std::string, Texture*> mTextures;
	std::map<std::string, std::vector<Texture*>> mAnimations;
};