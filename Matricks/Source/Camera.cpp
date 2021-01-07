/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#include "stdafx.h"

#include "SDL_scancode.h"
extern Engine* engine;
Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
	Reset();
}


namespace glm {
	bool operator <(const glm::vec2& l, const glm::vec2& r)
	{
		return l.x < r.x && l.y < r.y;
	}
	bool operator >(const glm::vec2& l, const glm::vec2& r)
	{
		return l.x > r.x && l.y > r.y;
	}
}

void Camera::Update()
{
	if (engine->IsPaused())
	{
		return;
	}
#ifdef DEBUG
	if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_C))
	{
		CameraControl = !CameraControl;
		if (CameraControl)
		{
			mTempPosition = mPosition;
		}
		else
		{
			mPosition = mTempPosition;
		}
	}
	if (CameraControl)
	{
		//Camera Scrolling
		if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_W))
		{
			mPosition.y -= 10.0f;
		}
		if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_S))
		{
			mPosition.y += 10.0f;
		}
		if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_D))
		{
			mPosition.x -= 10.0f;
		}
		if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_A))
		{
			mPosition.x += 10.0f;
		}
		
		//Camera Fast scrolling
		if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_W) && InputManager::GetInstance()->IsPressed(SDL_SCANCODE_LSHIFT))
		{
			mPosition.y -= 50.0f;
		}
		if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_S) && InputManager::GetInstance()->IsPressed(SDL_SCANCODE_LSHIFT))
		{
			mPosition.y += 50.0f;
		}
		if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_D) && InputManager::GetInstance()->IsPressed(SDL_SCANCODE_LSHIFT))
		{
			mPosition.x -= 50.0f;
		}
		if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_A) && InputManager::GetInstance()->IsPressed(SDL_SCANCODE_LSHIFT))
		{
			mPosition.x += 50.0f;
		}
	}

	ImGui::Begin("Camera");
	ImGui::Text("Press 'C' to toggle between CameraControl Mode and use 'WASD' to move Camera");
	ImGui::Text("Camera Position: %f %f",mPosition.x, mPosition.y);
	if(mPlayerTransform)
		ImGui::Text("Player Position %f %f", mPlayerTransform->mPosition.x, mPlayerTransform->mPosition.y);
	ImGui::End();
	//Zoom toggle
	if (INPUT_MANAGER->IsTriggered(SDL_SCANCODE_Z))
	{
		Zoom = !Zoom;
		if (Zoom)
		{
			LogInfo("ZoomIn");
			ZoomCamera(0, 1.5f);
		}
		else
		{
			LogInfo("ZoomOut");
			ZoomCamera(1, 1.5f);
		}
	}
	if (!CameraControl)
	{
#endif
		//TODO: Camera will move in FadeIn FIXME
		//Show final door
		if (mDelay < 0.0f)
		{
			LevelCamera = false;
		}
		else
		{
			mDelay -= PHYSICS_MANAGER->ReturnFrameTime();
		}
		if (!Zoom)
		{
			//Follow Player
			if (mPlayerTransform && mMoveinLevel && !LevelCamera)
			{
				MoveinX = -mPlayerTransform->mPosition.x;
				if (!Looking)
				{
					MoveinY = -mPlayerTransform->mPosition.y;
				}
			}
			//Moving in direction using Lerp
			if (fabs(MoveinX) > 0.0f)
			{
				float distance = (MoveinX - mPosition.x) * mLerpSpeed.x * PHYSICS_MANAGER->ReturnFrameTime();
				mPosition.x += distance;
				MoveinX -= distance;
			}
			if (fabs(MoveinY) > 0.0f)
			{
				float distance = (MoveinY - mPosition.y) * mLerpSpeed.y * PHYSICS_MANAGER->ReturnFrameTime();
				mPosition.y += distance;
				MoveinY -= distance;
			}
		}
#ifdef DEBUG
	}
#endif

	// Lerp Zooming WIP
	Zooming();

	//Events for Parallax backgrounds
	//if ((mLastPosition != mPosition && !CameraControl) || Zoom)
	{
		Event Cam(CAMERAMOVE, nullptr);
		EventManager::GetInstance()->BroadCastEventToSubscribers(&Cam);
	}
	
	mLastPosition = mPosition;
		
	mView = glm::scale(glm::mat4(1.0f), glm::vec3(mScale.x, mScale.y, 0.0f));
	mView = glm::translate(mView, glm::vec3(mPosition.x + mOffset.x, mPosition.y + mOffset.y, 0.0f));
}

void Camera::Init()
{
	if (GameObject* player = GameObjectManager::GetInstance()->GetPlayer())
	{
		mPlayerTransform = static_cast<Transform*>(player->GetComponent(TRANSFORM));
	}
	mPosition = mLastPosition;
	mView = glm::mat4(1.0f);
	mScale = mZoomTo = glm::vec2(1.0f);
	MoveinX = MoveinY = 0.0f;
	Zoom = false;
	Looking = false;
	LevelCamera = true;
	
	Event Cam(CAMERAMOVE, nullptr);
	EventManager::GetInstance()->BroadCastEventToSubscribers(&Cam);
}

void Camera::Serialize(rapidjson::Value::Object camera)
{
	if (camera.HasMember("Offset"))
	{
		rapidjson::Value::Array offset = camera["Offset"].GetArray();
		mOffset = glm::vec2(offset[0].GetFloat(), offset[1].GetFloat());
	}
	if (camera.HasMember("StartPos"))
	{
		rapidjson::Value::Array pos = camera["StartPos"].GetArray();
		mLastPosition = glm::vec2(pos[0].GetFloat(), pos[1].GetFloat());
		GameObjectManager::GetInstance()->camStartpos.x = mLastPosition.x;
		GameObjectManager::GetInstance()->camStartpos.y = mLastPosition.y;
	}
	if (camera.HasMember("Speed"))
	{
		rapidjson::Value::Array speed = camera["Speed"].GetArray();
		mLerpSpeed = glm::vec2(speed[0].GetFloat(), speed[1].GetFloat());
		mMoveinLevel = true;
	}
	else
	{
		mMoveinLevel = false;
	}
	if (camera.HasMember("Light"))
	{
		RENDER_MANAGER->SetAmbientLight(camera["Light"].GetFloat());
	}
	else
	{
		RENDER_MANAGER->SetAmbientLight(0.142f);
	}
}

void Camera::Reset()
{
	mView = glm::mat4(1.0f);
	mPlayerTransform = nullptr;
	mLastPosition = mPosition = mTempPosition = mOffset = glm::vec2(0.0f);
	CameraControl = false;
	LevelCamera = true;
	Looking = false;
	mLerpSpeed = glm::vec2(0.0f);
	MoveinY = MoveinX = 0.0f;
	mZoomPos = mZoomTo = glm::vec2(0.0f);
	mDelay = 1.0f;
	Zoom = false;
}

void Camera::MoveCamera(short direction, float distance, bool look)
{
	if (direction == 0)
		MoveinX = mPosition.x + distance;
	else if (direction == 1)
		MoveinY = mPosition.y + distance;
	Looking = look;
}

void Camera::ZoomCamera(short zoom, float factor, glm::vec2 position)
{
	if (!Zoom)
	{
		if (zoom == 0) //Zoomin
		{
			mZoomTo = mScale * factor;
		}
		else if (zoom == 1) //Zoomout
		{
			mZoomTo = mScale / factor;
		}
		if (mOffset == glm::vec2(0.0f))
			mZoomPos = -position + (glm::vec2(ENGINE_WIDTH,ENGINE_HEIGHT)/(2.0f + factor));
		else
			mZoomPos = -position + (glm::vec2(ENGINE_WIDTH, ENGINE_HEIGHT) / (2.0f + factor)) - mOffset;
		Zoom = true;
	}
}

void Camera::Zooming()
{
	if (mScale < mZoomTo - 0.01f || mScale > mZoomTo + 0.01f)
	{
		mScale += (mZoomTo.x - mScale.x) * PHYSICS_MANAGER->ReturnFrameTime();
		mPosition += (mZoomPos - mPosition) * PHYSICS_MANAGER->ReturnFrameTime();
	}
	else
		Zoom = false;
}

void Camera::SnaptoY()
{
	if (!Looking && !LevelCamera)
		MoveinY = -mPlayerTransform->mPosition.y;
}
