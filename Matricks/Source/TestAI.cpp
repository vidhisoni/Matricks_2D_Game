/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "TestAI.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "TriggerComponent.h"
#include  <math.h>

TestAI::TestAI() :Component(TESTAI)
{
	moveLeft = true;
	goUP = goDOWN = goBDOWN = goBUP = outofRange =
		playerDetected = jumpTrigger = moveRight =
		leftTrigger = rightTrigger = inAIR = false;
	pascalTriangle[0] = 1;
	pascalTriangle[1] = 2;
	pascalTriangle[2] = 1;
	once = 1;
	jumpBoxTrigger = jumpLeft = jumpRight = false;
}

void TestAI::Update()
{
	Transform *Enemytrans = static_cast<Transform *> (mpOwner->GetComponent(TRANSFORM));


	//Player direction check
	player = GameObjectManager::GetInstance()->GetPlayer();
	if (player != nullptr)
	{
// 		Transform *Playertrans = static_cast<Transform *> (player->GetComponent(TRANSFORM));
// 
// 		float temp = (Enemytrans->mScale.x*6.0f);
// 		if (Playertrans->mPosition.x - Enemytrans->mPosition.x >= -420.0f &&
// 			Playertrans->mPosition.x - Enemytrans->mPosition.x <= 0.0f &&
// 			abs(Playertrans->mPosition.y - Enemytrans->mPosition.y) <= 15.0f)
// 		{
// 			moveRight = false; moveLeft = true;
// 
// 		}
// 		else if (Enemytrans->mPosition.x - Playertrans->mPosition.x >= -420.0f &&
// 			Enemytrans->mPosition.x - Playertrans->mPosition.x <= 0.0f &&
// 			abs(Playertrans->mPosition.y - Enemytrans->mPosition.y) <= 15.0f)
// 		{
// 			moveRight = true; moveLeft = false;
// 		}

	}

	if (jumpTrigger) {
		Transform *t = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		Body *b = static_cast<Body*>(mpOwner->GetComponent(BODY));

		Transform *playerTransform = static_cast<Transform *>(player->GetComponent(TRANSFORM));

		outofRange = (playerTransform->mPosition.x - t->mPosition.x < 2000.0f) ? true : false;

		if (rightTrigger && !inAIR)
		{
			if (playerTransform->mPosition.x > t->mPosition.x
				&& playerTransform->mPosition.y >= t->mPosition.y) {
				goUP = true;
				calculateStep(alpha);
			}
			if ((playerTransform->mPosition.x > t->mPosition.x)
				&& playerTransform->mPosition.y < t->mPosition.y) {
				goDOWN = true;
				calculateStep(alpha);
			}
		}
		if (leftTrigger && !inAIR)
		{
			if (playerTransform->mPosition.x < t->mPosition.x
				&& playerTransform->mPosition.y < t->mPosition.y) {
				goBDOWN = true;
				calculateStep(alpha);
			}
			if (playerTransform->mPosition.x < t->mPosition.x
				&& playerTransform->mPosition.y >= t->mPosition.y) {
				goBUP = true;
				calculateStep(alpha);
			}
		}

		// JUMP Calculations
		if (goUP)
		{
			if (alpha < 1.0f) {
				if (once == 1)
					SetControlPoints(1);
				CalculateJump(alpha);
				setFlags(1);
			}
			else {
				ResetFlags();
			}
		}

		else if (goDOWN)
		{
			if (alpha < 1.0f) {
				if (once == 1)
					SetControlPoints(2);
				CalculateJump(alpha);
				setFlags(2);
			}
			else {
				ResetFlags();
			}
		}

		else if (goBDOWN)
		{

			if (alpha < 1.0f) {
				if (once == 1)
					SetControlPoints(3);
				CalculateJump(alpha);
				setFlags(3);
			}
			else {
				ResetFlags();
			}
		}

		else if (goBUP)
		{
			if (alpha < 1.0f) {
				if (once == 1)
					SetControlPoints(4);
				CalculateJump(alpha);
				setFlags(4);
			}
			else {
				ResetFlags();
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////

		//jump Over Num Box
	if (jumpBoxTrigger && !inAIR) {
		if (moveLeft) {
			jumpLeft = true;
			calculateStep(alpha);
		}
		if (moveRight) {
			jumpRight = true;
			calculateStep(alpha);
		}

	}

	if (jumpLeft)
	{
		if (alpha < 1.0f) {
			if (once == 1)
				SetControlPoints(5);
			CalculateJump(alpha);
			setFlags(1);
		}
		else {
			ResetFlags();
		}
	}
	if (jumpRight)
	{
		if (alpha < 1.0f) {
			if (once == 1)
				SetControlPoints(6);
			CalculateJump(alpha);
			setFlags(1);
		}
		else {
			ResetFlags();
		}

	}
	/////////////////////////////////////////////////////////////////////////////////////////

}

void TestAI::Clear()
{

}

Component* TestAI::Create()
{
	return new TestAI();
}

void TestAI::HandleEvent(Event* pEvent)
{

	if (pEvent->mType == AIMOVEMENT)
	{
		inAIR = false;
		if (mpOwner->GetType() == TESTAIBOT)
		{
			Transform *trans = static_cast<Transform *> (pEvent->pObject->GetComponent(TRANSFORM));

			rightLimit = trans->mPosition.x + trans->mScale.x / 2;
			leftLimit = trans->mPosition.x - trans->mScale.x / 2;

			Transform *t = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
			Body *b = static_cast<Body*>(mpOwner->GetComponent(BODY));

			if (moveRight)
			{
				t->mPosition.x += 2.5f;
				moveLeft = (t->mPosition.x > rightLimit) ? true : false;
				if (t->mPosition.x > rightLimit)
					t->mPosition.x = rightLimit - 10.0f;
			}

			if (moveLeft)
			{
				t->mPosition.x -= 2.5f;
				moveRight = (t->mPosition.x < leftLimit) ? true : false;
				if (t->mPosition.x < leftLimit)
					t->mPosition.x = leftLimit + 10.0f;
			}
			b->Initialize();
		}
	}

	if (pEvent->mType == TRIGGER)
	{
		//	if (playerDetected) {
		Transform *t = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));

		TriggerComponent *tCom = static_cast<TriggerComponent*>(pEvent->pObject->GetComponent(TRIGGERCOMPONENT));

		leftTrigger = tCom->left;
		rightTrigger = tCom->right;
		jumpTrigger = true;

		jumpstartPosX = t->mPosition.x;
		jumpstartPosY = t->mPosition.y;
		//	}
	}
	// 
	// 	if (pEvent->mType == PLAYERDETECT)
	// 	{
	// 		playerDetected = true;
	// 		speed = 4.5f;
	// 		outofRange = true;
	// 		
	// 	}

	// 	if (pEvent->mType == ENEMYHIT)
	// 	{
	// 		((Audio*)mpOwner->GetComponent(AUDIO))->Play(0);
	// 		if(pEvent->pObject->GetType() == PLAYER)
	// 			GameStateManager::GetInstance()->ChangeLevel(LEVEL2);
	// 	}

	if (pEvent->mType == BOX_AI)
	{
		Transform *t = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		Body *b = static_cast<Body*>(mpOwner->GetComponent(BODY));

		moveLeft = !moveLeft;
		moveRight = !moveRight;
		if (moveLeft)	t->mPosition.x -= 5.0f;
		if (moveRight)	t->mPosition.x += 5.0f;
		b->Initialize();

	}
	if (pEvent->mType == BOX_AI1)
	{
		Transform *t = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		Body *b = static_cast<Body*>(mpOwner->GetComponent(BODY));

		moveLeft = !moveLeft;
		moveRight = !moveRight;
		if (moveLeft)	t->mPosition.x -= 5.0f;
		if (moveRight)	t->mPosition.x += 5.0f;
		b->Initialize();

	}
	if (pEvent->mType == BOX_AI2)
	{
		Transform *t = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		Body *b = static_cast<Body*>(mpOwner->GetComponent(BODY));

		moveLeft = !moveLeft;
		moveRight = !moveRight;
		if (moveLeft)	t->mPosition.x -= 5.0f;
		if (moveRight)	t->mPosition.x += 5.0f;
		b->Initialize();

	}
	if (pEvent->mType == NUM_AI)
	{
		if (isDirty) {
			Transform *t = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
			//Body *b = static_cast<Body*>(mpOwner->GetComponent(BODY));
			jumpBoxTrigger = true;
			jumpNumStartX = t->mPosition.x;
			jumpNumStartY = t->mPosition.y;

			///*moveLeft = !moveLeft;
			//moveRight = !moveRight;*/
			/*if (moveLeft)	t->mPosition.x += 20.0f;
			if (moveRight)	t->mPosition.x -= 20.0f;
			b->Initialize();*/
			isDirty = false;
		}
	}


}

void TestAI::CalculateJump(float &alpha)
{
	float x, y;
	Transform *Enemytrans = static_cast<Transform *> (mpOwner->GetComponent(TRANSFORM));
	Body *eBody = static_cast<Body *> (mpOwner->GetComponent(BODY));

	if (alpha < 1.0f)
	{
		x = CurveX(alpha);
		Enemytrans->mPosition.x = x;
		//	std::cout << "x = " << x << std::endl;
		y = CurveY(alpha);
		Enemytrans->mPosition.y = y;
		//	std::cout << "y = " << y << std::endl;
		eBody->Initialize();
		alpha += 0.02778;
	}
}

void TestAI::calculateStep(float &aplha)
{
	steps = 6 * 6;
	alpha = 1 / steps;
}


void TestAI::setFlags(int direction)
{
	switch (direction) {

	case 1:
		goDOWN = false;
		goBDOWN = false;
		goBUP = false;
		inAIR = true;
		once++;
		break;
	case 2:
		goUP = false;
		goBUP = false;
		goBDOWN = false;
		inAIR = true;
		once++;
		break;
	case 3:
		goUP = false;
		goDOWN = false;
		goBUP = false;
		inAIR = true;
		once++;
		break;
	case 4:
		goUP = false;
		goDOWN = false;
		goBDOWN = false;
		inAIR = true;
		once++;
		break;
	}

}

void TestAI::ResetFlags()
{
	jumpTrigger = false;
	goUP = false;
	goDOWN = false;
	goBUP = false;
	goBDOWN = false;
	inAIR = false;
	alpha = 0.0f;
	once = 1;

	jumpBoxTrigger = false;
	jumpLeft = false;
	jumpRight = false;
	isDirty = true;
}

void TestAI::SetControlPoints(int direction)
{
	switch (direction) {

	case 1: // go Up
		cPoints[0][0] = jumpstartPosX;
		cPoints[0][1] = jumpstartPosY;

		cPoints[1][0] = jumpstartPosX + 150.0f;
		cPoints[1][1] = jumpstartPosY + 400.0f;

		cPoints[2][0] = jumpstartPosX + 400.f;
		cPoints[2][1] = jumpstartPosY + 20.0f;
		break;
	case 2: // go Down
		cPoints[0][0] = jumpstartPosX;
		cPoints[0][1] = jumpstartPosY;

		cPoints[1][0] = jumpstartPosX + 75.0f;
		cPoints[1][1] = jumpstartPosY + 200.0f;

		cPoints[2][0] = jumpstartPosX + 200.f;
		cPoints[2][1] = jumpstartPosY - 350.0f;
		break;
	case 3: // go bUp
		cPoints[0][0] = jumpstartPosX;
		cPoints[0][1] = jumpstartPosY;

		cPoints[1][0] = jumpstartPosX - 75.0f;
		cPoints[1][1] = jumpstartPosY + 200.0f;

		cPoints[2][0] = jumpstartPosX - 200.f;
		cPoints[2][1] = jumpstartPosY - 350.0f;
		break;
	case 4: // go bDown
		cPoints[0][0] = jumpstartPosX;
		cPoints[0][1] = jumpstartPosY;

		cPoints[1][0] = jumpstartPosX - 150.0f;
		cPoints[1][1] = jumpstartPosY + 400.0f;

		cPoints[2][0] = jumpstartPosX - 400.f;
		cPoints[2][1] = jumpstartPosY + 20.0f;
		break;
	case 5: // Left Box Jump
		cPoints[0][0] = jumpNumStartX;
		cPoints[0][1] = jumpNumStartY;

		cPoints[1][0] = jumpNumStartX - 35.0f;
		cPoints[1][1] = jumpNumStartY + 300.0f;

		cPoints[2][0] = jumpNumStartX - 300.0f;
		cPoints[2][1] = jumpNumStartY + 50.0f;
		break;
	case 6: // Right Box Jump
		cPoints[0][0] = jumpNumStartX;
		cPoints[0][1] = jumpNumStartY;

		cPoints[1][0] = jumpNumStartX + 35.0f;
		cPoints[1][1] = jumpNumStartY + 300.0f;

		cPoints[2][0] = jumpNumStartX + 300.0f;
		cPoints[2][1] = jumpNumStartY + 50.0f;
		break;
	}
}
float TestAI::CurveX(float t)
{
	float x = 0.0f;
	int degree = 2;

	for (int i = 0; i <= degree; ++i)
	{
		x += (float)(cPoints[i][0] * pascalTriangle[i]
			* pow((1 - t), (degree - i)) * pow(t, i));
	}

	return x;
}

float TestAI::CurveY(float t)
{
	int degree = 2;
	float y = 0.0f;
	for (int i = 0; i <= degree; ++i)
	{
		y += (float)(cPoints[i][1] * pascalTriangle[i]
			* pow((1 - t), (degree - i)) * pow(t, i));
	}
	return y;
}

