/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#include "LinerAI.h"
#include "AlarmComponent.h"
#include "key.h"
#include "TestAI.h"
#include "TriggerComponent.h"
#include "Sprite.h"
#include "Text.h"

#pragma comment(lib, "glu32.lib") 
#include "SDL_keyboard.h"
#include <windows.h>
#include <gl/GLU.h>

extern Engine* engine;

GameObjectManager* GameObjectManager::pObject = nullptr;

void GameObjectManager::Init()
{
	//Component creation map
	mComponentsMap[ANIMATOR] = new Animator();
 	mComponentsMap[TRANSFORM] = new Transform();
 	mComponentsMap[SPRITE] = new Sprite();
 	mComponentsMap[CONTROLLER] = new Controller();
 	mComponentsMap[BODY] = new Body();
	mComponentsMap[AUDIO] = new Audio();
	mComponentsMap[PARTICLE] = new ParticleEmitter();
	mComponentsMap[STATE] = new State();
	mComponentsMap[PLAYERSTATE] = new PlayerState();
	mComponentsMap[PARALLAX] = new ParallaxBg();
	mComponentsMap[SUBSCRIPTION] = new Subsciber();
	mComponentsMap[TEXT] = new Text();
	mComponentsMap[LIGHT] = new Light();
	mComponentsMap[CLOUD] = new Cloud();
	mComponentsMap[BUTTON] = new Button();
	mComponentsMap[SCRIPT] = new Script();
	mComponentsMap[LINERAI] = new LinerAI();
	mComponentsMap[ALARMCOMPONENT] = new AlarmComponent();
	mComponentsMap[KEY] = new key();
	mComponentsMap[TESTAI] = new TestAI();
	mComponentsMap[TRIGGERCOMPONENT] = new TriggerComponent();
	mComponentsMap[COLLIDER] = new Collider();
	mComponentsMap[PROMPT] = new Prompt();
	mComponentsMap[COMP_TRIGGER] = new Prompt_Trigger();
	mComponentsMap[STORY] = new Story();
	mComponentsMap[MENUCONTROL] = new MenuControl();
	mComponentsMap[LEVEL] = new LevelList();
	//Component creation map
	
#ifdef DEBUG
	std::string mFilePath = "Resources/Textures/BOX 6.png";

	newTexture = new Texture(mFilePath);

	bH[0] = 800.0f; bH[1] = 450.0f;
	bV[0] = 0.0f; bV[1] = 0.0f;
	camPos[0] = 0.0f; camPos[1] = 0.0f;
	
	bH[0] = 0.0f; levelbH[1] = 0.0f;
	levelbV[0] = 0.0f; levelbV[1] = 0.0f;
#endif
}

void GameObjectManager::Close()
{
	DestroyAll();
	ClearMenuStack();
	for (auto c : mComponentsMap)
	{
		delete c.second;
	}
	mComponentsMap.clear();
	delete pObject;
}

GameObjectManager * GameObjectManager::CreateInstance()
{
	pObject = new GameObjectManager();
	return pObject;
}

void GameObjectManager::Update()
{

#ifdef DEBUG
	// Working on  imgui in-game object editor
	const char* objName[] = { "NOOBJECT","PLAYER","PLATFORM","ENEMY","BACKGROUND","NUMBOX","IDOOR","BUTTONOBJ","TRAP",
							"FINALDOOR","INVISIPLATFORM","ALARM","BOUNCY","UnIDoor","AI_ENEMY","TESTAIBOT","TRIGGEROBJ",
							"STARTDOOR","PROMPTER","P_TRIGGER","BOX","CHARGE","NUMCLOUD","LIFT","VALUE","BOX_TRIGGER","PORTAL",
							"OUTOFBOUND","HUD_DOOR"};
	const char* pre = "Object";
	unsigned int x;
	static int selected = 0;


	if (buttonToggle) {
		ImGui::Begin("GameObjects");
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(1);
		ImGui::Separator();
		for (unsigned int i = 0; i < mGameObjects.size(); ++i)
		{
			Transform *mTrans = static_cast<Transform *>(mGameObjects[i]->GetComponent(TRANSFORM));
			Body *pBody = static_cast<Body *>(mGameObjects[i]->GetComponent(BODY));

			if (pBody != nullptr)
			{
				ImGui::PushID(i);
				ImGui::AlignTextToFramePadding();
				x = mGameObjects[i]->GetType();
				if (x < 26)
					ImGui::Text("%s", objName[x]);
				bool node_open = ImGui::TreeNode("Object", "Object_%i", i);

				ImGui::NextColumn();
				ImGui::AlignTextToFramePadding();
				if (node_open)
				{
					for (int x = 1; x < 3; ++x)
					{
						ImGui::PushID(i*x);
						ImGui::AlignTextToFramePadding();
						if (x == 1) {
							ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Bullet, "Transform_x");
							ImGui::DragFloat("##value", &mTrans->mPosition.x, 5.0f);
						}
						if (x == 2) {
							ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Bullet, "Transform_y");
							ImGui::DragFloat("##value", &mTrans->mPosition.y, 5.0f);
						}

						ImGui::PopID();
						pBody->Initialize();
					}
					ImGui::TreePop();
				}
				ImGui::PopID();
			}
		}
		ImGui::PopStyleVar();
		ImGui::End();
	}


	if (ObjectDetials)
	{
		
		static bool p_open = true;
		ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
		
		if(ImGui::Begin("Level Editor"))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Close")) ObjectDetials = false;
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			// left
			ImGui::BeginChild("left pane", ImVec2(150, 0), true);
			for (unsigned int i = 0; i < mGameObjects.size(); ++i)
			{
				char label[128];
			//	int y = mGameObjects[i]->GetType();
				//if(y > 0 && y < 24 )
					//sprintf(label, "_%s", objName[y]);	
			//	else
					sprintf(label, "Object_%d", i);

				if (ImGui::Selectable(label, selected == i))
					selected = i;
			}
			ImGui::EndChild();
			ImGui::SameLine();
			
			
			//Mouse selection
				ImGuiIO& io = ImGui::GetIO();
				int test = 0;
				if (ImGui::IsMouseClicked(test))
				{

					POINT mouse;
					HWND hWnd = nullptr;								 
					GetCursorPos(&mouse);                    
					ScreenToClient(hWnd, &mouse);

					GLfloat winX, winY, winZ, mouseX, mouseY;               

					mouseX = io.MousePos.x;
					mouseY = io.MousePos.y;

					GLint viewport[4];
					GLdouble modelview[16];
					GLdouble projection[16];

					GLdouble posX, posY, posZ;

					glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
					glGetDoublev(GL_PROJECTION_MATRIX, projection);
					glGetIntegerv(GL_VIEWPORT, viewport);

					winX = (float)mouseX;
					winY = (float)viewport[3] - (float)mouseY;;

					gluUnProject(winX, winY, 0.5, modelview, projection, viewport, &posX, &posY, &posZ);

					for (unsigned int i = 0; i < mGameObjects.size(); ++i) {

						if (mGameObjects[i]->GetType() != BACKGROUND && mGameObjects[i]->GetType() != NUMCLOUD &&
							mGameObjects[i]->GetType() != PROMPTER/* && mGameObjects[i]->GetType() != NOOBJECT*/)
						{
							Transform *mTrans = static_cast<Transform *>(mGameObjects[i]->GetComponent(TRANSFORM));

							glm::mat4 rayPos = RenderManager::GetInstance()->mProjection *
								RenderManager::GetInstance()->mCamera.mView *
								mTrans->GetDebugModel();


							float x = rayPos[0][0] / 2;
							float y = rayPos[1][1] / 2;
							float px = rayPos[3][0];
							float py = rayPos[3][1];
							bool case1 = posX <= px + x ? true : false;
							bool case2 = posX >= px - x ? true : false;
							bool case3 = posY >= py - y ? true : false;
							bool case4 = posY <= py + y ? true : false;
							if (case1 && case2 && case3 && case4)
							{
								//std::cout << "YES" << std::endl;
								selected = i;
							}
							else
							{
								//std::cout << "NO" << std::endl;
							}
						}
					}
				}


			//Change Texture after selecting objects
			if (mGameObjects[selected]->GetType() != BACKGROUND && mGameObjects[selected]->GetType() != VALUE && mGameObjects[selected]->GetType() != BOXTRIGGER) {
				Sprite *spriteCurrent = static_cast<Sprite *>(mGameObjects[selected]->GetComponent(SPRITE));


				if (previousSelection == selected) {
					if (i == 0)
						PrevTex = spriteCurrent->GetTexture();

					if (PrevTex != nullptr) {
						spriteCurrent->SetTexture(newTexture);
						changed = true;
					}
					i++;
				}
				else {
					Sprite* spritePrevious = static_cast<Sprite *>(mGameObjects[previousSelection]->GetComponent(SPRITE));
					if (spritePrevious != nullptr) {

						if (PrevTex != nullptr) {
							spritePrevious->SetTexture(PrevTex);
							changed = false;
						}


						previousSelection = selected;
						i = 0;
					}
				}
			}

			//// right
 			ImGui::BeginGroup();
 				ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
					x = mGameObjects[selected]->GetType();
					if (x < 26) {
						ImGui::Text("Object_type: %s", objName[x]);
					}
					else
						ImGui::Text("Object_: %d", selected);
			
 					ImGui::Separator();
 					if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
 					{
						Transform *mTrans = static_cast<Transform *>(mGameObjects[selected]->GetComponent(TRANSFORM));
						Body *pBody = static_cast<Body *>(mGameObjects[selected]->GetComponent(BODY));
						Sprite *pSprite = static_cast<Sprite *>(mGameObjects[selected]->GetComponent(SPRITE));
						Script *pScript = static_cast<Script *>(mGameObjects[selected]->GetComponent(SCRIPT));
						LinerAI *pAI = static_cast<LinerAI *>(mGameObjects[selected]->GetComponent(LINERAI));
						

 						if (ImGui::BeginTabItem("Editor"))
 						{
							if (pBody != nullptr) 
							{
								//TRANSFORM editor
								ImGui::PushID(selected);
								ImGui::AlignTextToFramePadding();
								bool node_open = ImGui::TreeNode("TRANSFORM");

								static int transValueX =  mTrans->mPosition.x;
								static int transValueY =  mTrans->mPosition.y;

								ImGuiIO& io = ImGui::GetIO();
								ImGui::NextColumn();
								ImGui::AlignTextToFramePadding();
								if (node_open)
								{
									for (int x = 1; x < 3; ++x)
									{
										ImGui::PushID(selected*x);
										ImGui::AlignTextToFramePadding();
										if (x == 1) {
											//ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Bullet, "Transform_x");
											ImGui::DragFloat("##value", &mTrans->mPosition.x, 5.0f);

// 											ImGui::Button("Drag X");
// 
// 											if (ImGui::IsItemActive())
// 											{
// 												
// 												ImDrawList* draw_list = ImGui::GetWindowDrawList();
// 												draw_list->PushClipRectFullScreen();
// 												draw_list->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
// 												draw_list->PopClipRect();
// 												float temp = mTrans->mPosition.x;
// 												
// 												ImVec2 value_raw = ImGui::GetMouseDragDelta(0, 0.0f);
// 											//	std::cout << mTrans->mPosition.x << std::endl;
// 												ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(0);
// 												ImVec2 mouse_delta = io.MouseDelta;
// 												ImGui::SameLine();
// 												
// 												transValueX = temp + value_raw.x;
// 											}
// 											ImGui::InputInt("Transform X", &transValueX);
// 											mTrans->mPosition.x = transValueX;
										}
										if (x == 2) {
											//ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Bullet, "Transform_y");
											ImGui::DragFloat("##value", &mTrans->mPosition.y, 5.0f);

// 											ImGui::Button("Drag Y");

											
// 
// 											if (ImGui::IsItemActive())
// 											{
// 												
// 												ImDrawList* draw_list = ImGui::GetWindowDrawList();
// 												draw_list->PushClipRectFullScreen();
// 												draw_list->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);
// 												draw_list->PopClipRect();
// 												float temp1 = mTrans->mPosition.y;
// 
// 												ImVec2 value_raw = ImGui::GetMouseDragDelta(0, 0.0f);
// 											//	std::cout << mTrans->mPosition.x << std::endl;
// 												ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(0);
// 												ImVec2 mouse_delta = io.MouseDelta;
// 												ImGui::SameLine();
// 												
// 												transValueY = temp1 + value_raw.y;
// 											}
// 
// 											ImGui::InputInt("Transform Y", &transValueY);
// 											mTrans->mPosition.y = transValueY;
										}

										ImGui::PopID();
										pBody->Initialize();
									}
									ImGui::TreePop();
								}
								ImGui::PopID();

								//SCALE editor
								ImGui::PushID(selected*6);
								ImGui::AlignTextToFramePadding();
								bool node_open1 = ImGui::TreeNode("SCALE");

								ImGui::NextColumn();
								ImGui::AlignTextToFramePadding();
								if (node_open1)
								{
									for (int x = 3; x < 5; ++x)
									{
										ImGui::PushID(selected*x);
										ImGui::AlignTextToFramePadding();

										if (x == 3) {
											//ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Bullet, "Scale_x");
											ImGui::DragFloat("##value", &mTrans->mScale.x, 5.0f);
										}
										if (x == 4) {
											//ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Bullet, "Scale_y");
											ImGui::DragFloat("##value", &mTrans->mScale.y, 5.0f);
										}

										ImGui::PopID();
										pBody->Initialize();
									}
									ImGui::TreePop();
								}
								ImGui::PopID();

								if (mGameObjects[selected]->GetType() == PLATFORM)
								{
									//Tiling for platform
									ImGui::PushID(selected * 8);
									ImGui::AlignTextToFramePadding();
									bool node_open2 = ImGui::TreeNode("TILING");
									static int tiling[] = { pSprite->mData.Tiles.x, pSprite->mData.Tiles.y };

									ImGui::NextColumn();
									ImGui::AlignTextToFramePadding();
									if (node_open2)
									{
										for (int x = 5; x < 6; ++x)
										{
											ImGui::PushID(selected*x);
											ImGui::AlignTextToFramePadding();

											if (x == 5) {
											//	ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Bullet, "Tiling");
												ImGui::InputInt2("Tiling", tiling);
											}
											pSprite->mData.Tiles.x = tiling[0];
											pSprite->mData.Tiles.y = tiling[1];
											ImGui::PopID();
											pBody->Initialize();
										}
										ImGui::TreePop();
									}
									ImGui::PopID();
								}

								//Delete Objects 
 								ImGui::PushID(selected * 9);
 								ImGui::AlignTextToFramePadding();
 								bool node_open2 = ImGui::TreeNode("DELETE OBJECT");
 
 								ImGui::NextColumn();
 								ImGui::AlignTextToFramePadding();
 								if (node_open2)
 								{
									if (ImGui::Button("Delete"))
									{
										mGameObjects[selected]->Destroy();
										for (unsigned int i = 0; i < mGameObjects.size(); ++i)
										{
											if (mGameObjects[i]->GetType() == BACKGROUND) {
												selected = i;
												break;
											}
										}
									}
									ImGui::TreePop();
 								}
 								ImGui::PopID();
								
								// TRAP editor
								if (mGameObjects[selected]->GetType() == TRAP)
								{
									//Limit Change for TRAP
									ImGui::PushID(selected * 10);
									ImGui::AlignTextToFramePadding();
									bool node_open3 = ImGui::TreeNode("Trap LIMITS");
									static float UDlimit[] = { pScript->upLimit, pScript->downLimit };
									static float LRlimit[] = { pScript->leftLimit, pScript->rightLimit };
									static float speed = pScript->speed;
									int temp = 0;
									if (UDlimit[0] != 0.0f)	temp = 1;
									if (LRlimit[0] != 0.0f) temp = 2;
									ImGui::NextColumn();
									ImGui::AlignTextToFramePadding();
									if (node_open3)
									{
										for (int x = 11; x < 12; ++x)
										{
											ImGui::PushID(selected*x);
											ImGui::AlignTextToFramePadding();

											if (x == 11 && temp == 1) {
												//ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Bullet, "Tiling");
												ImGui::InputFloat("UpLimit", &UDlimit[0]);
												ImGui::InputFloat("DownLimit", &UDlimit[1]);
												ImGui::InputFloat("Speed", &speed);
											}
											if (x == 11 && temp == 2) {
												//ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Bullet, "Tiling");
												ImGui::InputFloat("LeftLimit", &LRlimit[0]);
												ImGui::InputFloat("RightLimit", &LRlimit[1]);
												ImGui::InputFloat("Speed", &speed);
											}
											pScript->upLimit = UDlimit[0]; pScript->downLimit = UDlimit[1];
											pScript->leftLimit = LRlimit[0]; pScript->rightLimit = LRlimit[1];
											pScript->speed = speed;
											ImGui::PopID();
										}
										ImGui::TreePop();
									}
									ImGui::PopID();
								}
							}
							ImGui::EndTabItem();
						}

 						if (ImGui::BeginTabItem("Details"))
 						{
							if (pBody != nullptr) {
								ImGui::Text("---------------BODY---------------");
								glm::vec2 temp; float f;
								temp = pBody->ReturnPos();
								ImGui::Text("Pos X : %f", temp.x);
								ImGui::Text("Pos Y : %f", temp.y);

								ImGui::InputFloat("Body Mass", &pBody->rReturnMass());	
								ImGui::InputFloat("Inve Mass", &pBody->rReturnInvMass());

								temp = pBody->ReturnAcceleration();
								ImGui::Text("Acceleration X : %f", temp.x);
								ImGui::Text("Acceleration Y : %f", temp.y);

								temp = pBody->ReturnVelocity();
								ImGui::Text("Velocity X : %f", temp.x);
								ImGui::Text("Velocity Y : %f", temp.y);
								temp = pBody->ReturnScale();
								ImGui::Text("Scale X : %f", temp.x);
								ImGui::Text("Scale Y : %f", temp.y);
								temp = pBody->ReturnFrictionCoeff();
								ImGui::Text("Friction X : %f", temp.x);
								ImGui::Text("Friction Y : %f", temp.y);
								//ImGui::Text("Bounce Factor : %f", pBody->ReturnBounceFactor());

								ImGui::PushID(selected);
								ImGui::AlignTextToFramePadding();
								bool node_open = ImGui::TreeNode("Bounce Factor");

								ImGui::NextColumn();
								ImGui::AlignTextToFramePadding();
								if (node_open)
								{
									for (int x = 1; x < 2; ++x)
									{
										ImGui::PushID(selected*x);
										ImGui::AlignTextToFramePadding();
										if (x == 1) {
											ImGui::DragFloat("##value", &pBody->rReturnBounceFactor(), 0.1f);
										}

										ImGui::PopID();
										pBody->Initialize();
									}
									ImGui::TreePop();
								}
								ImGui::PopID();
								pBody->Initialize();

							}
							if (pSprite != nullptr) {
								ImGui::Text("---------------SPRITE---------------");
								std::string t = pSprite->ReturnTextype();
								const char* tem = t.c_str();
								ImGui::Text("Sprite Name : %s", tem);
							}
							if (pScript != nullptr) {
								ImGui::Text("---------------SCRIPT---------------");
								std::string t = pScript->GetScript();
								const char* tem = t.c_str();
								ImGui::Text("Script Name : %s", tem);
							}
							if (pAI != nullptr) {
								ImGui::Text("---------------AI---------------");
								ImGui::Text("RIGHTLIMIT  : %f", pAI->rightLimit);
								ImGui::Text("LEFTLIMIT : %f", pAI->leftLimit);
							}
 							ImGui::EndTabItem();
 						}





						if (ImGui::BeginTabItem("ADD Objects"))
						{
							static float trans[] = { 0.0f, 0.0f };
							static float scale[] = { 0.0f, 0.0f };
							static int tiling[] = { 0, 0 };
							static int textValue = 0;
							static float speed = 0.0f;
							static float childTrans[] = {0.0f, 0.0f};
							static float childscale[] = { 0.0f, 0.0f };
							static float UDlimit[] = {0.0f, 0.0f};
							static float LRLimit[] = {0.0f, 0.0f};
							const char* objName[] = {"BOX","INFINITEBOX","PLATFORM","TRAP_UP","TRAP_LEFT","IDOOR","FINALDOOR","STARTDOOR",
													 "BOUNCY SURFACE"};
 							for (unsigned int i = mGameObjects.size(); i < mGameObjects.size() + 9; ++i)
 							{
 								ImGui::PushID(i);
								ImGui::AlignTextToFramePadding();

								int temp = i - mGameObjects.size();
 								bool node_open = ImGui::TreeNode("Object", "%s", objName[temp]);
								ImGui::NextColumn();
								ImGui::AlignTextToFramePadding();
 								if (node_open)
  								{
 									for (int x = 1; x < 3; ++x)
  									{
  										ImGui::PushID(i*x);
  										ImGui::AlignTextToFramePadding();
 										if (x == 1) 
										{
											ImGui::InputFloat2("Transform", trans);
  										}
										if(x == 2) {
											ImGui::InputFloat2("Scale", scale);
										}
										if (x == 2 && temp == 0) {
											ImGui::InputInt("Text Value", &textValue);
										}
										if (x == 2 && temp == 2){
											ImGui::InputInt2("Tiling", tiling);
										}
										if (x == 2 && temp == 3) {

											ImGui::InputFloat("Up_Limit", &UDlimit[0]);									
											ImGui::InputFloat("Down_Limit", &UDlimit[1]);
											ImGui::InputFloat("Speed", &speed);

										}
										if (x == 2 && temp == 4) {
											ImGui::InputFloat("Left_Limit", &LRLimit[0]);
											ImGui::InputFloat("Right_Limit", &LRLimit[1]);
											ImGui::InputFloat("Speed", &speed);
										}
										if (x == 2 && temp == 5) {
											ImGui::InputInt("System Value", &textValue);
											ImGui::InputFloat2("Child Transform", childTrans);
											ImGui::InputFloat2("Child Scale", childscale);
										}
										if (x == 2 && temp == 6) {
											ImGui::InputInt("Final Door Value", &textValue);
										}


										if (x == 2 && temp == 0)
										{
											if (ImGui::Button("BOX"))
											{
												GameObject* obj1 = GameObjectFactory::GetInstance()->LoadObject("Box.json");
												Text* pText = static_cast<Text*>(obj1->GetComponent(TEXT));
												pText->SetText(std::to_string(textValue));
												ObjectCreationHelper(obj1, trans, scale);
											}
										}
										if (x == 2 && temp == 1)
										{
											if (ImGui::Button("INFINITEBOX"))
											{
												GameObject* obj1 = GameObjectFactory::GetInstance()->LoadObject("InfiniteBox.json");
												ObjectCreationHelper(obj1, trans, scale);
											}
										}
										if (x == 2 && temp == 2)
										{
											if (ImGui::Button("PLATFORM"))
											{
												GameObject* obj1 = GameObjectFactory::GetInstance()->LoadObject("Platform.json");
												Sprite* pSp = static_cast<Sprite*>(obj1->GetComponent(SPRITE));
												pSp->mData.Tiles.x = tiling[0];
												pSp->mData.Tiles.y = tiling[1];
												ObjectCreationHelper(obj1, trans, scale);
											}
										
										}	
										if (x == 2 && temp == 3)
										{
											if (ImGui::Button("TRAP_Up"))
											{
												GameObject* obj1 = GameObjectFactory::GetInstance()->LoadObject("trapUp.json");
												Script *pScript = static_cast<Script *>(obj1->GetComponent(SCRIPT));
												pScript->upLimit = UDlimit[0];
												pScript->downLimit = UDlimit[1];
												pScript->speed = speed;
												ObjectCreationHelper(obj1, trans, scale);
											}

										}
										if (x == 2 && temp == 4)
										{
											if (ImGui::Button("TRAP_Left"))
											{
												GameObject* obj1 = GameObjectFactory::GetInstance()->LoadObject("trapLeft.json");
												Script *pScript = static_cast<Script *>(obj1->GetComponent(SCRIPT));
												pScript->leftLimit = LRLimit[0];
												pScript->rightLimit = LRLimit[1];
												pScript->speed = speed;
												ObjectCreationHelper(obj1, trans, scale);
											}

										}
										if (x == 2 && temp == 5)
										{
											if (ImGui::Button("IDOOR"))
											{
												GameObject* obj1 = GameObjectFactory::GetInstance()->LoadObject("IDoor.json");
												Text* pText = static_cast<Text*>(obj1->GetComponent(TEXT));
												pText->SetText(std::to_string(textValue));
												ObjectCreationHelper(obj1, trans, scale);

												//Child obj
												GameObject* childObj = obj1->ChildFind(INVISIPLATFORM);
												ChildObject(childObj, childTrans, childscale);
												
											}
										}
										if (x == 2 && temp == 6)
										{
											if (ImGui::Button("FINAL DOOR"))
											{
												GameObject* obj1 = GameObjectFactory::GetInstance()->LoadObject("FinalDoor.json");
												Text* pText = static_cast<Text*>(obj1->GetComponent(TEXT));
												pText->SetText(std::to_string(textValue));
												ObjectCreationHelper(obj1, trans, scale);
											}
										}
										if (x == 2 && temp == 7)
										{
											if (ImGui::Button("START DOOR"))
											{
												GameObject* obj1 = GameObjectFactory::GetInstance()->LoadObject("StartDoor.json");
												ObjectCreationHelper(obj1, trans, scale);
											}
										}
										if (x == 2 && temp == 8)
										{
											if (ImGui::Button("BOUNCY SURFACE"))
											{
												GameObject* obj1 = GameObjectFactory::GetInstance()->LoadObject("Platform Bouncy.json");
												ObjectCreationHelper(obj1, trans, scale);
											}
										}
  										ImGui::PopID();
 // 									//pBody->Initialize();
  									}
 									ImGui::TreePop();
 								}
 								ImGui::PopID();
							}
		
							ImGui::EndTabItem();

						}


						if (ImGui::BeginTabItem("New LEVEL"))
						{
							
  							const char* objName1[] = { "Camera","Level" };
  							if (ImGui::Button("CREATE NEW LEVEL"))
  							{
  								GameStateManager::GetInstance()->ChangeLevel(LEVELTEMPLATE); 
 							}
  							for (unsigned int i = mGameObjects.size() + 6; i < mGameObjects.size() + 7; ++i)
  							{
  								ImGui::PushID(i);
  								ImGui::AlignTextToFramePadding();
  
  								int temp = i - (mGameObjects.size()+6);
  								bool node_open = ImGui::TreeNode("Object", "%s", objName1[temp]);
  								ImGui::NextColumn();
  								ImGui::AlignTextToFramePadding();
  								if (node_open)
  								{
  									for (int x = 1; x < 2; ++x)
  									{
  										ImGui::PushID(i*x);
  										if (x == 1 && temp == 0) {
  											ImGui::InputFloat2("Cam_StartPos", camPos);
  										}
  
  										ImGui::AlignTextToFramePadding();
  										ImGui::PopID();							
  									}
  									ImGui::TreePop();
  								}
  
  								ImGui::PopID();
  							}

							if (ImGui::Button("DeSerialize"))
							{
								DeSerialize("savefile");
							}
							ImGui::EndTabItem();

						}


						if (ImGui::BeginTabItem("Level Selector"))
						{
						
							const std::string objName[] = { "Level_1","Level_2","Level_3","Level_4","Level_5",
													"Level_6","Level_7","Level_8","Level_9","Level_10",
													"Level_11","Level_12","Level_13","Level_14"};

							static int selected = 0;
							ImGui::BeginChild("left pane", ImVec2(150, 0), true);
							for (unsigned int i = 1; i < 15; ++i)
							{
								char label[128];
								sprintf(label, "LEVEL_%d", i);

								if (ImGui::Selectable(label, selected == i))
									selected = i;
								
							}
							ImGui::EndChild();
							ImGui::SameLine();

							
							ImGui::BeginGroup();
							ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

							if (ImGui::Button("Load Level"))
							{
								GameStateManager::GetInstance()->ChangeLevel(static_cast<Levels>(selected));
							}
							if (ImGui::Button("Save As"))
							{
								std::string name = objName[selected-1];
								DeSerialize("Levels/"+name);
							}
							ImGui::EndChild();
							ImGui::EndGroup();
							ImGui::EndTabItem();

						}

 						ImGui::EndTabBar();
 					}
				ImGui::EndChild();
 			ImGui::EndGroup();

		}
		ImGui::End();
	}

	if (!ObjectDetials && changed)
	{
		Sprite* spritePrevious = static_cast<Sprite *>(mGameObjects[previousSelection]->GetComponent(SPRITE));
		if (spritePrevious != nullptr && PrevTex != nullptr) {
			spritePrevious->SetTexture(PrevTex);
			changed = false;
		}
	}
#endif
// 
// 	if (GAMESTATE_MANAGER->GetCurrentLevel() != SETTINGS)
// 		engine->inSettings = false;


	if (screenRes.x == 1024 && screenRes.y == 576)	{ res1 = true; res2 = false; res3 = false; }
	if (screenRes.x == 1366 && screenRes.y == 768)	{ res1 = false; res2 = true; res3 = false; }
	if (screenRes.x == 1920 && screenRes.y == 1080)	{ res1 = false; res2 = false; res3 = true; }

	for (unsigned int i = 0; i < mGameObjects.size(); ++i)
	{
		mGameObjects[i]->Update();
		if (mGameObjects[i]->GetType() == BUTTONOBJ)
		{
			Button* pButton = static_cast<Button*>(mGameObjects[i]->GetComponent(BUTTON));
			Sprite* mSprite = static_cast<Sprite*>(mGameObjects[i]->GetComponent(SPRITE));
			if (pButton->type1) { 
				if(res1) pButton->selected = true; 
				else pButton->selected = false; 
			}
			if (pButton->type2) {
				if (res2) pButton->selected = true;
				else pButton->selected = false;
			}
			if (pButton->type3) {
				if (res3) pButton->selected = true;
				else pButton->selected = false;
			}

			if (pButton->muteButtonBGM && AudioManager::GetInstance()->muteBGM)
				mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("AudioMute"));
			if (pButton->muteButtonBGM && !AudioManager::GetInstance()->muteBGM)
				mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("AudioUnMute"));


			if (pButton->muteButtonSFX && AudioManager::GetInstance()->muteSFX) 
				mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("AudioMute"));
			if (pButton->muteButtonSFX && !AudioManager::GetInstance()->muteSFX)
				mSprite->SetTexture(RESOURCE_MANAGER->GetTexture("AudioUnMute"));

		}
	}
}

// Destroy the game objects for which the "isDestroyed" flag is set
void GameObjectManager::DestoryQ()
{
	//Faster Removing
	for (unsigned int i = 0; i < mGameObjects.size(); ++i)
	{
		if (mGameObjects[i]->isDestroyed())
		{
			MEMORY_MANAGER->FreeGameObject(mGameObjects[i]);
			mGameObjects.erase(mGameObjects.begin() + i);
		}
	}
}

void GameObjectManager::DestroyAll()
{
	for (auto go : mGameObjects)
		MEMORY_MANAGER->FreeGameObject(go);

	mGameObjects.clear();
}

void GameObjectManager::RemoveInGameMenu()
{
	
	for (auto g : *mMenuStack.top())
	{
		(*std::find(mGameObjects.begin(), mGameObjects.end(), g))->Destroy();
	}
	delete mMenuStack.top();
	mMenuStack.pop();
	if (mMenuStack.size() > 0)
	{
		for (auto g : *mMenuStack.top())
		{
			(*std::find(mGameObjects.begin(), mGameObjects.end(), g))->mActive = true;
		}
	}
	else
		engine->UnPause();
}

void GameObjectManager::ChangeInGameMenu()
{
	if (mMenuStack.size() > 0)
	{
		for (auto g : *mMenuStack.top())
		{
			(*std::find(mGameObjects.begin(), mGameObjects.end(), g))->mActive = false;
		}
	}
	else if (GAMESTATE_MANAGER->GetCurrentLevel() == MAINMENU)
	{
		std::vector<GameObject*> *list = new std::vector<GameObject *>();
		for (auto g : mGameObjects)
		{
			if (g->GetType() == BUTTONOBJ)
			{
				g->mActive = false;
				list->push_back(g);
			}
		}
		mMenuStack.push(list);
	}
}

void GameObjectManager::EmptyMenuStack()
{
	while (mMenuStack.size() > 0)
	{
		for (auto g : *mMenuStack.top())
			(*std::find(mGameObjects.begin(), mGameObjects.end(), g))->Destroy();
		delete mMenuStack.top();
		mMenuStack.pop();
	}
}

void GameObjectManager::ClearMenuStack()
{
	while (mMenuStack.size() > 0)
	{
		delete mMenuStack.top();
		mMenuStack.pop();
	}
}

void GameObjectManager::Pause()
{
	//Set isPaused of active gameobjects to true and Load Menu GameObjects
	for (auto g : mGameObjects)
	{
		if(g->GetType() != BACKGROUND)
			g->isPaused = true;
	}
}

void GameObjectManager::UnPause()
{
	//Delete the objects with isPaused = false and set isPaused = true to false
	for (auto g : mGameObjects)
	{
		if (g->isPaused)
			g->isPaused = false;
		else if (g->GetType() != BACKGROUND)
		{
			g->Destroy();
		}
	}
	if (mMenuStack.size() > 0)
	{
		delete mMenuStack.top();
		mMenuStack.pop();
	}
}

GameObject* GameObjectManager::GetPlayer()
{
	for (auto g : mGameObjects)
	{
		if (g->GetType() == PLAYER)
			return g;
	}
	return nullptr;
}

std::vector<GameObject *> GameObjectManager::GetValue()
{
	std::vector<GameObject *> temp;
	for (auto g : mGameObjects)
	{
		if (g->GetType() == VALUE)
			temp.push_back(g);
	}
	return temp;
}

void GameObjectManager::DeSerialize(std::string FileName)
{
	std::ofstream savefile;
	std::string name = "Resources/Data/" + FileName + ".json";
	savefile.open(name);

	//Camera Deserialize
	savefile << "{\"Camera\":{";
	savefile << "\"StartPos\":[" << camStartpos.x << "," << camStartpos.y << "]";
	savefile << "\},";



	//GameObjects Deserialize
	savefile << "\"Objects\":[";
	for (auto g : mGameObjects)
	{
		if (g->mParentObject == nullptr)
		{
			savefile << "{\"File\":\"" << g->GetFileName() << "\",";
			LevelDeSerialize(&savefile, g);
		}
	}
	long pos2 = (long)savefile.tellp();
	savefile.seekp(pos2 - 1);
	savefile << "]}";
	savefile.close();
	LogInfo("Deserialized to File");
}



void GameObjectManager::LevelDeSerialize(std::ofstream* file, GameObject* obj)
{
	if (!obj->mChildObjects.empty())
	{
		*file << "\"Children\":[";
		for (unsigned int i = 0; i < obj->mChildObjects.size(); ++i)
		{
			*file << "{\"Index\":" << i << ",";
			LevelDeSerialize(file, obj->mChildObjects[i]);
		}
		long pos2 = (long)file->tellp();
		file->seekp(pos2 - 1);
		*file << "],";
	}
	
	for (auto c : GAMESTATE_MANAGER->mLevelSerialize)
	{
		Component* pComp = nullptr;
		if(pComp = obj->GetComponent(c))
		switch (c)
		{
			case TRANSFORM:
				static_cast<Transform*>(pComp)->DeSerialize(file);
				break;
			case SPRITE:
				static_cast<Sprite*>(pComp)->DeSerialize(file);
				break;
			case TEXT:
				static_cast<Text*>(pComp)->DeSerialize(file);
				break;
			case PLAYERSTATE:
				static_cast<PlayerState*>(pComp)->DeSerialize(file);
				break;
			case BODY:
				static_cast<Body*>(pComp)->DeSerialize(file);
				break;
			case SCRIPT:
				static_cast<Script*>(pComp)->DeSerialize(file);
				break;
		}
	}
	//remove previous ','
	long pos = (long)file->tellp();
	file->seekp(pos - 1);
	*file << "},";
}

void GameObjectManager::ObjectCreationHelper(GameObject * obj1, float tran[], float scale[])
{

	Transform* ptr1 = static_cast<Transform*>(obj1->GetComponent(TRANSFORM));
	Body* pBody1 = static_cast<Body*>(obj1->GetComponent(BODY));
	Sprite* pSp = static_cast<Sprite*>(obj1->GetComponent(SPRITE));


	ptr1->mPosition.x = tran[0];
	ptr1->mPosition.y = tran[1];
	ptr1->mAngle = 0.0f;
	ptr1->mScale.x = scale[0];
	ptr1->mScale.y = scale[1];

	pBody1->Initialize();

	pSp->SetZCoord(0.3f);
}

void GameObjectManager::ChildObject(GameObject * obj1, float trans[] , float scale[])
{

	Transform* ptr1 = static_cast<Transform*>(obj1->GetComponent(TRANSFORM));

	Sprite* pSp = static_cast<Sprite*>(obj1->GetComponent(SPRITE));


	ptr1->mPosition.x = trans[0];
	ptr1->mPosition.y = trans[1];
	ptr1->mAngle = 0.0f;
	ptr1->mScale.x = scale[0];
	ptr1->mScale.y = scale[1];


	pSp->SetZCoord(0.3f);
}
