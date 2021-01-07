/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once

#include "Component.h"
#include <List>
#include <List.h>
#include <iterator>

class TestAI : public Component
{
public:
	TestAI();

	void Update();
	void Clear();
	Component* Create();
	void HandleEvent(Event* pEvent);

	//Helper functions
	float CurveX(float t);
	float CurveY(float t);
	void CalculateJump(float &);
	void calculateStep(float &);
	void ResetFlags();
	void setFlags(int);
	void SetControlPoints(int);

public:

	float cPoints[3][3];
	float jumpstartPosX, jumpstartPosY, speed,
		rightLimit, leftLimit, playerX, playerY,
		steps, alpha;
	int pascalTriangle[3];
	int once;
	bool jumpTrigger, playerDetected, goUP, goDOWN,
		goBUP, goBDOWN, outofRange, leftTrigger,
		rightTrigger, inAIR, moveRight, moveLeft,
		activated;
	GameObject* player = nullptr;
	bool jumpBoxTrigger, jumpLeft, jumpRight;
	int jumpNumStartX, jumpNumStartY;
	bool isDirty = true;
};