/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"

GameObject::GameObject()
{
	mParentObject = nullptr;
	mHash = 0;
	emitParticle = false;
	isPaused = false;
	mActive = true;
}

void GameObject::Update()
{
	if (mActive)
	{
		for (auto c : mComponents) {
			c->Update();
		}
	}
}

Component *GameObject::AddComponent(unsigned int Type)

{
	GameObjectManager* gm = GameObjectManager::GetInstance();
	Component *pNewComponent = nullptr;

	// TO DO add "create" function for every component
	//pNewComponent = GameObjectManager::GetInstance()->mComponentsMap[Type]->Create();
	pNewComponent = MemoryManager::GetInstance()->NewComponent((ComponentType)Type);

	if (nullptr != pNewComponent)
	{
		mComponents.push_back(pNewComponent);
		// TO DO 
		pNewComponent->mpOwner = this;		
	}

	return pNewComponent;

}

Component *GameObject::GetComponent(unsigned int Type)
{
	for (auto c : mComponents)
		if (c->GetType() == Type) {
			return c;
		}
	return nullptr;
}

void GameObject::HandleEvent(Event * pEvent)
{
	for (auto c : mComponents)
		c->HandleEvent(pEvent);
	//Send Event to Childobjects too
	for (auto ch : mChildObjects)
		ch->HandleEvent(pEvent);
}


void GameObject::Clear()
{
	this->mObjectType = NOOBJECT; 
	for (auto c : mComponents) {
		MemoryManager::GetInstance()->FreeComponent(c);
	}
	mComponents.clear();
	if (!mChildObjects.empty())
	{
		for (auto g : mChildObjects) {
			g->Destroy();
		}
	}
	mChildObjects.clear();
	if (mParentObject)
	{
		mParentObject->DeleteChild(this);
		mParentObject = nullptr;
	}

	mDestroyed = false;
	isPaused = false;
	mActive = true;

	mHash = 0;
}

GameObject * GameObject::ChildFind(GameObjectType find)
{
	for (auto Child : mChildObjects)
	{
		if (Child->GetType() == find)
			return Child;
	}
	return nullptr;
}

void GameObject::DeleteChild(GameObject* child)
{
	auto itr = std::find(mChildObjects.begin(), mChildObjects.end(), child);
	if (itr != mChildObjects.end())
		mChildObjects.erase(itr);
}