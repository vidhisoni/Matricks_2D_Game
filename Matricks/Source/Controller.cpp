/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
Modified by: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "SDL_scancode.h"
#include"SDL_gamecontroller.h"

Controller::Controller() : Component(CONTROLLER), AudioDebug(false)
{

}

void Controller::Update()
{
	if (test == 1)
	{
		if (nullptr != mpOwner && nullptr != InputManager::GetInstance())
		{
			Audio *audio = static_cast<Audio *>(mpOwner->GetComponent(AUDIO));
			Body *pBody = static_cast<Body *>(mpOwner->GetComponent(BODY));

			PlayerState *pState = static_cast<PlayerState*>(mpOwner->GetComponent(PLAYERSTATE));
#ifdef RELEASE
			//if( nullptr != pState && !InputManager::GetInstance()->mControllerActive)
#endif
			{
				if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_LEFT)) {

					//audio->Play(0);
					if (pBody->rRetrunAction()[InAir] == false/* && !isPromptAudio*/) {
						audio->Play(0);
					}
					pState->ChangeState(MOVELEFT);
				}
				if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_RIGHT)) {
					//audio->Play(0);
					if (pBody->rRetrunAction()[InAir] == false/* && !isPromptAudio*/) {
						audio->Play(0);
					}
					pState->ChangeState(MOVERIGHT);
				}

				if (InputManager::GetInstance()->IsReleased(SDL_SCANCODE_LEFT))
				{
					audio->Stop();					
					pState->ChangeState(IDLELEFT);
				}
				if (InputManager::GetInstance()->IsReleased(SDL_SCANCODE_RIGHT))
				{
					audio->Stop();
					pState->ChangeState(IDLERIGHT);
				}
				if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_M))
				{
					AudioManager::GetInstance()->muteBGM = !AudioManager::GetInstance()->muteBGM;
				}

				if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_UP))
				{
					RenderManager::GetInstance()->mCamera.MoveCamera(1, -400.0f, true);
				}
				if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_DOWN))
				{
					RenderManager::GetInstance()->mCamera.MoveCamera(1, 400.0f, true);
				}
				if (InputManager::GetInstance()->IsReleased(SDL_SCANCODE_DOWN) || InputManager::GetInstance()->IsReleased(SDL_SCANCODE_UP))
				{
					RenderManager::GetInstance()->mCamera.NotLooking();
				}


#ifdef DEBUG
				ImGui::Begin("Audio");
				if (ImGui::Button("AudioMute"))
					AudioDebug = !AudioDebug;
				ImGui::SameLine();
				ImGui::Text(" %s", AudioDebug ? "ON" : "OFF");
				ImGui::End();

				if (AudioDebug)
					AudioManager::GetInstance()->MuteMusic();
				else
					AudioManager::GetInstance()->UnMuteMusic();
#endif

				//THIS IS FOR VARIABLE JUMP
				/*if ((InputManager::GetInstance()->IsReleased(SDL_SCANCODE_SPACE))) {
					pState->ChangeState(JUMP);
					pBody->rReturnJumpC()++;
				}
				if (InputManager::GetInstance()->IsPressed(SDL_SCANCODE_SPACE)) {
				}*/
				//THIS IS FOR VARIABLE JUMP

				//THIS IS FOR STATIC JUMP
				if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_SPACE) && pBody->rReturnJumpC() < pBody->ReturnMaxJumps()) {
					audio->StopSFX(0);					
					audio->Play(4);
					pState->ChangeState(JUMP);
					pBody->rReturnJumpC()++;
					int temp = (int)AudioManager::GetInstance()->SFX_Volume;
					int temp1 = (int)AudioManager::GetInstance()->Audio_Volume;
					AudioManager::GetInstance()->SetSFXsVolume(temp);
					//std::cout << "SFX " << temp << std::endl;
					//std::cout << "BGM " << temp1 << std::endl;
				}
			}
#ifdef RELEASE
			//else
				//if (nullptr != pState && InputManager::GetInstance()->mControllerActive)
#endif
			{
				if (InputManager::GetInstance()->IsButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
					//audio->Play(0);
					if (pBody->rRetrunAction()[InAir] == false) {
						audio->Play(0);
					}
					pState->ChangeState(MOVELEFT);
				}
				if (InputManager::GetInstance()->IsButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
					//audio->Play(0);
					if (pBody->rRetrunAction()[InAir] == false) {
						audio->Play(0);
					}
					pState->ChangeState(MOVERIGHT);
				}

				if (InputManager::GetInstance()->IsButtonReleased(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
				{
					audio->StopSFX(0);
					pState->ChangeState(IDLELEFT);
				}
				if (InputManager::GetInstance()->IsButtonReleased(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
				{
					audio->StopSFX(0);
					pState->ChangeState(IDLERIGHT);
				}
				if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_UP))
				{
					RenderManager::GetInstance()->mCamera.MoveCamera(1, -400.0f, true);
				}
				if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_DOWN))
				{
					RenderManager::GetInstance()->mCamera.MoveCamera(1, 400.0f, true);
				}
				if (InputManager::GetInstance()->IsReleased(SDL_SCANCODE_DOWN) || InputManager::GetInstance()->IsReleased(SDL_SCANCODE_UP))
				{
					RenderManager::GetInstance()->mCamera.NotLooking();
				}
				//TODO: (Sindhu) Move this to Input Manager or Have Imgui controls for Audio
				if (InputManager::GetInstance()->IsTriggered(SDL_SCANCODE_M)) {
					AudioManager::GetInstance()->ToggleMuteMusic();
				}
				//THIS IS FOR STATIC JUMP
				if (InputManager::GetInstance()->IsButtonTriggered(SDL_CONTROLLER_BUTTON_A) && pBody->rReturnJumpC() < pBody->ReturnMaxJumps()) {
					audio->Stop();
					audio->StopSFX(0);
					pState->ChangeState(JUMP);
					pBody->rReturnJumpC()++;
				}
			}
		}
	}

}

void Controller::DeSerialize(std::ofstream* file)
{
	*file << "{\n\"Type\":\"Controller\"\n},\n";
}

void Controller::Clear()
{

}

Component* Controller::Create()
{
	return new Controller();
}

void Controller::HandleEvent(Event* pEvent)
{

}
