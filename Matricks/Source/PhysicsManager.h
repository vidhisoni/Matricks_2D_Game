/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once
#define gravity glm::vec2(0.0f,-2200.0f)
class NSquared;
class DynamicTree;
class PhysicsManager
{
	static PhysicsManager* pObject;
	PhysicsManager() { }
public:

	static PhysicsManager* CreateInstance();
	inline static PhysicsManager* GetInstance() { return pObject; }
	void StateSetter(float alpha);
	void Update();
	void Init() { mFPS = mFrameTime = 0.0f; }
	void Close();
	void SetFPS(float _fps);//The physics engine is deterministic
	float ReturnFrameTime();

	NSquared nsq;//The slow N Squared way of checking collision
	DynamicTree DynTree;//Dynamic AABB tree
private:
	float mFPS;
	float mFrameTime;
};