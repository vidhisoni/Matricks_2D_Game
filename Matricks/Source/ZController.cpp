#include "stdafx.h"
#include "SDL_keyboard.h"
#include "SDL_mouse.h"

ZController::ZController() : Component(ZCONTROLLER)
{

}

ZController::~ZController()
{

}

Component* ZController::Create()
{
	return new ZController();
}

void ZController::Update()
{
	InputManager* pInputManager = InputManager::GetInstance();
	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));

	pTr->mAngle = getAngleFromMouse(pTr->mPosition);

	if (pInputManager->IsPressed(SDL_SCANCODE_UP))
	{
		pTr->mPosition.y += 8.0f;
		pTr->mAngle = 90.0f;
		//move up
	}
	if (pInputManager->IsPressed(SDL_SCANCODE_LEFT))
	{
		pTr->mPosition.x -= 8.0f;
		pTr->mAngle = 180.0f;
		//move left
	}
	if (pInputManager->IsPressed(SDL_SCANCODE_DOWN))
	{
		pTr->mPosition.y -= 8.0f;
		pTr->mAngle = 270.0f;
		//move down
	}
	if (pInputManager->IsPressed(SDL_SCANCODE_RIGHT))
	{
		pTr->mPosition.x += 8.0f;
		pTr->mAngle = 0.0f;
		//move right
	}
	if (pInputManager->IsPressed(SDL_SCANCODE_RIGHT) && pInputManager->IsPressed(SDL_SCANCODE_DOWN)) {
		pTr->mAngle = 315.0f;
	}
	if (pInputManager->IsPressed(SDL_SCANCODE_RIGHT) && pInputManager->IsPressed(SDL_SCANCODE_UP)) {
		pTr->mAngle = 45.0f;
	}
	if (pInputManager->IsPressed(SDL_SCANCODE_LEFT) && pInputManager->IsPressed(SDL_SCANCODE_UP)) {
		pTr->mAngle = 135.0f;
	}
	if (pInputManager->IsPressed(SDL_SCANCODE_LEFT) && pInputManager->IsPressed(SDL_SCANCODE_DOWN)) {
		pTr->mAngle = 225.0f;
	}
	if (pInputManager->IsMouseClicked(SDL_BUTTON_LEFT))
	{
		//shoot
		GameObject* pBullet = GameObjectFactory::GetInstance()->GetArcheType(BULLET);
		Transform* pTb = static_cast<Transform*>(pBullet->GetComponent(TRANSFORM));
		Body* pBody = static_cast<Body*>(pBullet->GetComponent(BODY));
		pTb->mScale = glm::vec2(50.0f, 10.0f);
		pTb->mPosition = pTr->mPosition;
		pTb->mAngle = pTr->mAngle;
		pTb->mZCoord = pTr->mZCoord;
		pBody->Initialize();
	}

	//Temporary code
	if (fabs(-pTr->mPosition.x - RenderManager::GetInstance()->mCamera.GetPosition().x) < 6.0f)
	{
		RenderManager::GetInstance()->mCamera.MoveCamera(0, 2*1920.0f);
	}
	if (fabs(-pTr->mPosition.x + 1920.0f - RenderManager::GetInstance()->mCamera.GetPosition().x) < 6.0f)
	{
		RenderManager::GetInstance()->mCamera.MoveCamera(0, 2*-1920.0f);
	}
	if (fabs(-pTr->mPosition.y - RenderManager::GetInstance()->mCamera.GetPosition().y) < 6.0f)
	{
		RenderManager::GetInstance()->mCamera.MoveCamera(1, 2 * 1080.0f);
	}
	if (fabs(-pTr->mPosition.y + 1080.0f - RenderManager::GetInstance()->mCamera.GetPosition().y) < 6.0f)
	{
		RenderManager::GetInstance()->mCamera.MoveCamera(1, 2 * -1080.0f);
	}
}

void ZController::Clear()
{

}


float ZController::getAngleVector(glm::vec2 mVec1, glm::vec2 aVec2)
{

	float dotProduct, angle;
	glm::vec2 UpVec;

	dotProduct = glm::dot(mVec1,aVec2);
	UpVec = glm::vec2(-mVec1.y, mVec1.x);


	angle = acosf(dotProduct / (glm::length(mVec1)*glm::length(aVec2)));

	if (glm::dot(UpVec, aVec2) < 0)
	{
		angle = -angle;
	}
	return angle;
}

float ZController::getAngleFromMouse(glm::vec2 PlayerPos)
{
	glm::vec2 Dir, eDir;
	glm::vec2 MouseVec = InputManager::GetInstance()->GetMouseLocation();
	Dir = MouseVec - PlayerPos;
	eDir = glm::vec2((float)cosf(180*radians), (float)sinf(180*radians));
	Dir = glm::normalize(Dir);
	float ang = getAngleVector(Dir, eDir);
	if (ang > 0)
		return (float)acosf(Dir.x) * 1/ radians;
	else
		return (float)-(acosf(Dir.x) * 1/radians);
}


