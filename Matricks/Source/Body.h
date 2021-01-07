/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once
class Shape;
enum action
{
	GoingLeft,
	GoingRight,
	OnGround,
	InAir,
	Jumping,
	NUM
};

class Body:public Component
{
public:
	bool collider;

public:
	Body();
	~Body();
	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);
	void Initialize();
	void Clear();
	void HandleEvent(Event* pEvent);
	void Update();
	Component* Create();


	bool ScreenOutY();
	bool ScreenOutX();
	bool GroundTouchCheck();

	
	void StopX();
	void ApplyForceX(float ForceX);
	void StopY();
	void ApplyForceY(float ForceY);

	void Integrate(float deltaTime);    //Integrator with time deltaTime

	glm::vec2 ReturnPos();				//value
	glm::vec2& rReturnPos();			//reference
	
	glm::vec2 ReturnScale();		    //value
	glm::vec2& rReturnScale();          //reference 

	glm::vec2 ReturnVelocity();	        //value
	glm::vec2& rReturnVelocity();		//reference

	glm::vec2 ReturnOldVelocity();		//Value

	float ReturnMass();			        //value
	float& rReturnMass();				//reference
	
	float ReturnInvMass();		        //value
	float& rReturnInvMass();		        //reference

	glm::vec2 ReturnPrevPos();	        //value
	glm::vec2 ReturnAcceleration();		//value
	
	glm::vec2 ReturnTotalForce();		//value
	glm::vec2&rReturnTotalForce();     //reference
	
	glm::vec2 ReturnFrictionCoeff();	//value x is static y is dynamic
	glm::vec2& rReturnFrictionCoeff();  //Reference X is static y is dynamic

	glm::vec2 ReturnXBound();			//value
	glm::vec2 ReturnYBound();			//value
	glm::vec2& rReturnXBound();			//reference
	glm::vec2& rReturnYBound();			//reference
	float ReturnBounceFactor();			//Value
	float& rReturnBounceFactor();		//refernece

	int ReturnMaxJumps();				//Value
	int& rReturnJumpC();				//reference to jump counter

	Shape* ReturnShape();               //return the shape

	bool* rRetrunAction();				//return the Action boolean array
	void PosUpdate(float frametime);
	void ShapeUpdate();

	bool ReturnCollider();				//value

	//TODO: Make Private
private:
	glm::vec2 mVel;
	float mBounceFactor;
	glm::vec2 mTotalForce;
	bool mAction[NUM];
	int mMaxJumps;

	int mJumpCount;

	Shape *mpShape;

	glm::vec2 mPos;
	
	glm::vec2 mFrictionCoeff;

	glm::vec2 mPrevPos;
	glm::vec2 mScaleFactor;

	glm::vec2 mOldVel;

	glm::vec2 mXBounds;
	glm::vec2 mYBounds;

	glm::vec2 mAccel;
	float mMass,mInvMass;
	bool bTouchingGround;
	glm::vec2 mScale;
};