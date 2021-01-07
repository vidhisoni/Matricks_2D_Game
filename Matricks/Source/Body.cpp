/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#include "stdafx.h"

Body::Body() :Component(BODY)
{
	mPos = glm::vec2(0.0f);
	mPrevPos = glm::vec2(0.0f);
	mVel = glm::vec2(0.0f);
	mAccel = glm::vec2(0.0f);
	mTotalForce = glm::vec2(0.0f);
	mXBounds = glm::vec2(0.0f);
	mFrictionCoeff = glm::vec2(0.0f);
	mBounceFactor = 0.0f;
	mMass = 0.0f;
	mInvMass = 0.0f;
	mAction[NUM];
	mpShape = nullptr;
	bTouchingGround = false;
	mJumpCount = 0;
	mScale = glm::vec2(0.0f);
	mYBounds = glm::vec2(0.0f);
	mMaxJumps = 0;
	collider = false;
	mOldVel = glm::vec2(0.0f);
}

Body::~Body()
{
	delete mpShape;
}

void Body::Serialize(rapidjson::Value::Object obj)
{
	if (obj.HasMember("Vel")) {
		rapidjson::Value::Array Vel = obj["Vel"].GetArray();
		mVel.x = Vel[0].GetFloat();
		mVel.y = Vel[1].GetFloat();
	}
	if (obj.HasMember("Accel")) {
		rapidjson::Value::Array  Accel = obj["Accel"].GetArray();
		mAccel.x = Accel[0].GetFloat();
		mAccel.y = Accel[1].GetFloat();
	}
	if (obj.HasMember("Mass")) {
		mMass = obj["Mass"].GetFloat();
		if (mMass != 0.0f)
			mInvMass = 1 / mMass;
		else
			mInvMass = 0.0f;
	}
	if (obj.HasMember("Bounce")) {
		mBounceFactor = obj["Bounce"].GetFloat();
	}
	if (obj.HasMember("Friction")) {
		rapidjson::Value::Array  Friction = obj["Friction"].GetArray();
		mFrictionCoeff.x = Friction[0].GetFloat();
		mFrictionCoeff.y = Friction[1].GetFloat();
	}

	if (obj.HasMember("AABB")) {
		rapidjson::Value::Array  Scale = obj["AABB"].GetArray();
		mpShape = new ShapeAABB();
		mpShape->mpOwnerBody = this;
		mScaleFactor.x = Scale[0].GetFloat();
		mScaleFactor.y = Scale[1].GetFloat();
	}
	if (obj.HasMember("CIRCLE")) {
		float Scale = obj["CIRCLE"].GetFloat();
		mpShape = new ShapeCircle();
		mpShape->mpOwnerBody = this;
		mScaleFactor.x = Scale;
		mScaleFactor.y = Scale;
	}

	if (obj.HasMember("Jumps")) {
		mMaxJumps = obj["Jumps"].GetInt();
	}
	if (obj.HasMember("Collider")) {
		collider = obj["Collider"].GetBool();
	}
	Initialize();
}


void Body::DeSerialize(std::ofstream* file)
{
	if (mMaxJumps != 0)
	{
		*file << "\"Body\":{";
		*file << "\"Jumps\":" << mMaxJumps;
		*file << "},";
	}
}


void Body::Initialize()
{
	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
	if (pTr == nullptr)
		return;
	mPos = pTr->mPosition;
	mPrevPos = pTr->mPosition;
	mScale = mScaleFactor * pTr->mScale;
	if (mpShape->mType == Shape::ShapeType::AABB)
	{
		ShapeAABB *pAABB = static_cast<ShapeAABB*>(mpShape);
		pAABB->Initialize(mScale, mPos);
	}
	if (mpShape->mType == Shape::ShapeType::CIRCLE)
	{
		ShapeCircle *pCircle = static_cast<ShapeCircle*>(mpShape);
		pCircle->Initialize(mScale, mPos);
	}
}

void Body::Clear()
{
	mPos = glm::vec2(0.0f);
	mPrevPos = glm::vec2(0.0f);
	mVel = glm::vec2(0.0f);
	mAccel = glm::vec2(0.0f);
	mTotalForce = glm::vec2(0.0f);
	mXBounds = glm::vec2(0.0f);
	mFrictionCoeff = glm::vec2(0.0f);
	mBounceFactor = 0.0f;
	mMass = 0.0f;
	mInvMass = 0.0f;
	mAction[NUM];
	mpShape = nullptr;
	bTouchingGround = false;
	mJumpCount = 0;
	mScale = glm::vec2(0.0f);
	mYBounds = glm::vec2(0.0f);
	mMaxJumps = 0;
	collider = false;
	mOldVel = glm::vec2(0.0f);
}

void Body::HandleEvent(Event* pEvent)
{
	if (pEvent->mType == ONGROUNDCOLLISION)
	{
		bTouchingGround = true;
	}
	if (pEvent->mType == CHARGEPICKUP && mpOwner->GetType() == CHARGE)
	{
		if(mpOwner!= nullptr)
			mpOwner->Destroy();
	}
}

void Body::Update()
{
	if (mpOwner->GetType() == P_TRIGGER)
	{
		//mPos = ((Body*)mpOwner->mParentObject->GetComponent(BODY))->mPos;//When using P_TRIGGER this is how to use it
	}
}

void Body::ApplyForceX(float ForceX)
{
	mTotalForce.x += ForceX;
}

void Body::ApplyForceY(float ForceY)
{
	mTotalForce.y = ForceY;
	mVel.y = 0.0f;
}

Component* Body::Create()
{
	return new Body();
}

bool Body::ScreenOutX()
{
	if (mPos.x - (mScale.x*mScaleFactor.x) <= mXBounds.x)
		return true;
	if (mPos.x + (mScale.x*mScaleFactor.x) >= mXBounds.y)
		return true;
	else
	return false;
}

bool Body::ScreenOutY()
{
	if (mPos.y + (mScale.y*mScaleFactor.y) >= mYBounds.y)
		return true;
	else 
		return false;
}

bool Body::GroundTouchCheck()
{
	if (mPos.y - 0.5*mScale.y < 0.0f)
		return true;
	else
		return false;
}

glm::vec2 Body::ReturnPos()
{
	return mPos;
}

glm::vec2 Body::ReturnScale()
{
	return mScale;
}

glm::vec2 & Body::rReturnScale()
{
	return mScale;
}

glm::vec2 Body::ReturnVelocity()
{
	return mVel;
}

float Body::ReturnMass()
{
	return mMass;
}

float & Body::rReturnMass()
{
	return mMass;
}

float Body::ReturnInvMass()
{
	return mInvMass;
}

float & Body::rReturnInvMass()
{
	return mInvMass;
}

glm::vec2 Body::ReturnPrevPos()
{
	return mPrevPos;
}

glm::vec2 Body::ReturnAcceleration()
{
	return mAccel;
}

glm::vec2 Body::ReturnTotalForce()
{
	return mTotalForce;
}

glm::vec2 & Body::rReturnTotalForce()
{
	return mTotalForce;
}

glm::vec2 Body::ReturnFrictionCoeff()
{
	return mFrictionCoeff;
}

glm::vec2 & Body::rReturnFrictionCoeff()
{
	return mFrictionCoeff;
}

glm::vec2 Body::ReturnXBound()
{
	return mXBounds;
}

glm::vec2 Body::ReturnYBound()
{
	return mYBounds;
}

glm::vec2 & Body::rReturnXBound()
{
	return mXBounds;
}

glm::vec2 & Body::rReturnYBound()
{
	return mYBounds;
}

float Body::ReturnBounceFactor()
{
	return mBounceFactor;
}

float & Body::rReturnBounceFactor()
{
	return mBounceFactor;
}

int Body::ReturnMaxJumps()
{
	return mMaxJumps;
}

glm::vec2 & Body::rReturnVelocity()
{
	return mVel;
}

glm::vec2 Body::ReturnOldVelocity()
{
	return mOldVel;
}

glm::vec2 & Body::rReturnPos()
{
	return mPos;
}

int & Body::rReturnJumpC()
{
	return mJumpCount;
}

Shape * Body::ReturnShape()
{
	return mpShape;
}

bool* Body::rRetrunAction()
{
	return mAction;
}

void Body::PosUpdate(float frametime)
{
	mPos += mVel * frametime;
	if (mpOwner->mParentObject)
	{
		mPos = static_cast<Body*>(mpOwner->mParentObject->GetComponent(BODY))->ReturnPos();
	}
	ShapeUpdate();
}

void Body::ShapeUpdate()
{
	if (mpShape->mType == Shape::ShapeType::AABB)
	{
		ShapeAABB* m = static_cast<ShapeAABB*>(mpShape);
		m->rReturnPos() = mPos;
	}
	else if (mpShape->mType == Shape::ShapeType::CIRCLE)
	{
		ShapeCircle* m = static_cast<ShapeCircle*>(mpShape);
		m->rReturnPos() = mPos;
	}
}

bool Body::ReturnCollider()
{
	return collider;
}

void Body::StopX()
{
	mVel.x = -(mVel.x)*.50f;
	mPos.x = mPrevPos.x;
	mTotalForce.x = 0.0f;
}

void Body::StopY()
{
	mVel.y = -(mVel.y)*.50f;
	mPos.y = mPrevPos.y;
	mTotalForce.y = 0.0f;
}

void Body::Integrate(float DeltaTime)
{
	//Save current position is PrevPos
	mPrevPos = mPos;
	
	//Create Drag Force 
	glm::vec2 drag(0.0f);
	
	if (mAction[InAir])
	{
		drag = -mVel*0.01f;//Apply Drag only if the object is InAir
	}

	glm::vec2 physics_constraints;

	physics_constraints = drag;// Add other force later on if you need more physics forces like wind etc

	//Total force is Force applied + other constraints
	mTotalForce = mTotalForce + physics_constraints;

	//Compute acceleration
	mAccel = mTotalForce * mInvMass;

	glm::vec2 Gravity=glm::vec2(0.0f, -2200.0f);// This is an absurd number because the movement is pixel based
	
	if (mInvMass == 0.0f)
	// If the object has infinite mass i.e. InvMass=0.0f,
	//then dont apply gravity to them as they are supposed to be stationary objects
	Gravity = glm::vec2(0.0f);

	//Apply the gravity to the acceleration
	mAccel = mAccel + Gravity;

	//Compute Velocity
	mOldVel = mVel;
	mVel += mAccel * DeltaTime;
	
	if (abs(mVel.x) > 350.0f)//Capping the max velocity to 350.0f
	{
		mVel.x = mOldVel.x;
	}

	//Set the total force to 0 again
	mTotalForce.y =0.0f;
}


