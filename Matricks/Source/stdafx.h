// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Sai Sindhu Jannali
//- End Header --------------------------------------------------------
#pragma once

#include "GL/glew.h"
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <time.h>
#include <filesystem>
#include <thread>
#include <chrono>

//STL Containers
#include <map>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include <deque>
#include <stack>

//GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define radians 0.0174533f

//Imgui
#ifdef DEBUG
#include "Imgui/imgui.h"
#include "Imgui/imconfig.h"
#include "Imgui/imgui_impl_sdl.h"
#include "Imgui/imgui_impl_opengl3.h"
#endif

#include "rapidjson/document.h"

#include "OpenGL/Texture.h"
#include "OpenGL/Renderer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/FrameBuffer.h"


#include"fmod.hpp"
#include"fmod_errors.h"

//MANAGERS
#include "Engine.h"
#include"Camera.h"
#include "TextRenderer.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "FrameRateController.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "GameStateManager.h"
#include "MemoryManager.h"
#include"BroadPhase.h"
#include"NSquared.h"
#include"DynamicAABBTree.h"
#include "PhysicsManager.h"
#include "ResourceManager.h"
#include "GameObjectFactory.h"
#include "ScriptHandler.h"

#include "ParticleSystem.h"


#include"Log.h"
#include <LuaBridge.h>

//COMPONENTS
#include"Component.h"
#include"Transform.h"
#include"Body.h"
#include"Button.h"
#include"Controller.h"
#include"Cloud.h"
#include"Light.h"
#include"ParallaxBg.h"
#include"PlayerState.h"
#include"Script.h"
#include "ParticleEmitter.h"
#include"Sprite.h"
#include"State.h"
#include"Subscriber.h"
#include"Text.h"
#include"Animator.h"
#include"Audio.h"
#include "LinerAI.h"
#include "AlarmComponent.h"
#include "AI_Enemy.h"
#include"Collider.h"
#include "Prompt.h"
#include "Prompt_Trigger.h"
#include "Story.h"
#include "MenuControl.h"
#include "Levels.h"