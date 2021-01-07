/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#include "stdafx.h"
#define GLM_ENABLE_EXPERIMENTAL
#include"glm/gtx/norm.hpp"

//TODO: (Shantanu) Collider
PhysicsManager* PhysicsManager::pObject = nullptr;

void PositionalCorrection(Contact* Manifold)
{
	const float slop = 0.01f; //The SLOP(Error tolerance)
	glm::vec2 percent(0.6, 0.6);
	glm::vec2 correction;
	//float penetration = std::max(Manifold->penetration - slop, 0.0f) / (Manifold->mBodies[0]->ReturnInvMass() + Manifold->mBodies[1]->ReturnInvMass());
	correction = (std::max(Manifold->penetration - slop, 0.0f) / (Manifold->mBodies[0]->ReturnInvMass() + Manifold->mBodies[1]->ReturnInvMass()))
		*percent*Manifold->ContactNormal;
	//Correction is amount of distance correction that is to be applied in the direction of contact,
	//scaled by correction percentage and the penetration distance

	//Finally apply the correction factor to the position
	Manifold->mBodies[0]->rReturnPos() -= correction * Manifold->mBodies[0]->ReturnInvMass();
	Manifold->mBodies[1]->rReturnPos() += correction * Manifold->mBodies[1]->ReturnInvMass();
}

void ApplyFriction(Contact *Manifold, float Impulse)
{
	//Calculate the relative velocity
	glm::vec2 rv;
	rv = Manifold->mBodies[1]->ReturnVelocity() - Manifold->mBodies[0]->ReturnVelocity();//VB-VA

	//Calculating the tangent vector
	glm::vec2 tangent= rv-(glm::dot(rv, Manifold->ContactNormal)*Manifold->ContactNormal);
	//Normalizing it
	tangent = glm::normalize(tangent);
	
	//Check if the normalise yields a nan
	if ((isnan(tangent.x) && isnan(tangent.y))||(isinf(tangent.x)&&isinf(tangent.y)))
		return;

	if (isnan(tangent.x)|| isinf(tangent.x))
	{
		tangent.x = 0.0f;
	}
	if (isnan(tangent.y)||isinf(tangent.y))
	{
		tangent.y = 0.0f;
	}

	//Magnitude of the tangent impulse
	float jT = -glm::dot(rv, tangent);//-negative as friction impulse will be applied in the opposite direction

	jT /= (Manifold->mBodies[0]->ReturnInvMass() + Manifold->mBodies[1]->ReturnInvMass());//Setting the magnitude of the impulse according to the inverse masses

	float mu = (Manifold->mBodies[1]->ReturnFrictionCoeff().x);
	//ReturnFrictionCoeff().x is static ReturnFrictionCoeff().y is dynamic

	glm::vec2 frictionImpulse;
	//If the impulse cannot break the static friction then apply the static friction
	if (abs(jT) < Impulse*mu)
	{
		frictionImpulse = jT* tangent;
	}
	else//Apply the dynamic friction
	{
		float dynamicFriciton = (Manifold->mBodies[1]->ReturnFrictionCoeff().y);
		frictionImpulse = -Impulse * tangent*dynamicFriciton;
	}
	if (isnan(frictionImpulse).x && isnan(frictionImpulse.y))
		return;
	if (isnan(frictionImpulse).x)
		frictionImpulse.x = 0.0f;
	if (isnan(frictionImpulse).y)
		frictionImpulse.y = 0.0f;

	//Apply impulse to the bodies
	Manifold->mBodies[0]->rReturnVelocity() -= frictionImpulse * Manifold->mBodies[0]->ReturnInvMass();
	Manifold->mBodies[1]->rReturnVelocity() += frictionImpulse * Manifold->mBodies[1]->ReturnInvMass();

}

void ResolveCollision(Contact* Manifold)
{
	// Calculate relative velocity
	glm::vec2 rv;
	rv = Manifold->mBodies[1]->ReturnVelocity() - Manifold->mBodies[0]->ReturnVelocity();//VB-VA

	// Calculate relative velocity in terms of the normal direction
	float velAlongNormal = glm::dot(rv, Manifold->ContactNormal);
	// Do not resolve if velocities are separating
	if (velAlongNormal > 0)
		return;

	// Calculate restitution
	//float e = min(A.restitution, B.restitution)//Average could have been calculated but right now just taking the bounce of the second object
	float e = (Manifold->mBodies[1]->ReturnBounceFactor());

	// Calculate impulse scalar
	float j = -(1 + e) * velAlongNormal;
	j /= Manifold->mBodies[0]->ReturnInvMass() + Manifold->mBodies[1]->ReturnInvMass();

	// Calculate the actual impulse in the direction of the contact normal
	glm::vec2 impulse = Manifold->ContactNormal* j;

	//Apply Impulse to the bodies 
	Manifold->mBodies[0]->rReturnVelocity() -= impulse * Manifold->mBodies[0]->ReturnInvMass();
	Manifold->mBodies[1]->rReturnVelocity() += impulse * Manifold->mBodies[1]->ReturnInvMass();
	
	//To make that impulse is applied accoring to the type of the bodies(wrt to mass of the individual bodies)
	/*glm::vec2 impulse = Manifold->ContactNormal * j;
	float mass_sum = Manifold->mBodies[0]->ReturnMass() + Manifold->mBodies[1]->ReturnMass();
	float ratio = Manifold->mBodies[0]->ReturnMass() / mass_sum;
	if (ratio == 1.0f)
		ratio = Manifold->mBodies[0]->ReturnInvMass();

	Manifold->mBodies[0]->rReturnVelocity()-= impulse * ratio;
	
	ratio = Manifold->mBodies[1]->ReturnMass() / mass_sum;
	if (ratio == 1.0f)
		ratio = Manifold->mBodies[1]->ReturnInvMass();
	Manifold->mBodies[1]->rReturnVelocity()+= impulse * ratio;
	*/
	//To make that impulse is applied accoring to the type of the bodies(wrt to mass of the individual bodies){COULDNT GET THIS TO WORK!}

	//Move on to applying friciton
	ApplyFriction(Manifold, j);
}



PhysicsManager* PhysicsManager::CreateInstance()
{
	pObject = new PhysicsManager();
	return pObject;
}


void PhysicsManager::StateSetter(float alpha)
//Used to slightly manipulate the rendering objects to the physics update
//if multiple physics update happen in a frame
{
	for (auto go : GameObjectManager::GetInstance()->mGameObjects)
	{
		Body *pBody = static_cast<Body*>(go->GetComponent(BODY));
		Transform *pTrans= static_cast<Transform*>(go->GetComponent(TRANSFORM));
		if (pBody != nullptr && !pBody->mpOwner->isPaused)
		{
			pBody->rReturnTotalForce() = glm::vec2(0.0f);
			ShapeAABB* pAABB = static_cast<ShapeAABB*>(pBody->ReturnShape());
			if (pAABB == nullptr)
				continue;
			pTrans->mPosition = pBody->ReturnPrevPos() * (1.0f - alpha) + pBody->ReturnPos() * alpha;
			pAABB->rReturnPos() = pTrans->mPosition;
		}
	}
}

void PhysicsManager::Update()
{
	//Reset all the Contact Manifolds
	CollisionManager::GetInstance()->Reset();
	//Broad Phase computation
	DynTree.Update();
	DynTree.ComputePairs();
	//Broad Phase end

	//nsq.ComputePairs();//The nsqaured was being called before dynamic AABB
	for (auto pair : DynTree.m_pairs)//Passing the pair generated in broadphase to narrow phase for contact generation
	{//The DynTree.m_pairs was 	nsq.m_pairs for nsquared

		//NARROW PHASE
		CollisionManager::GetInstance()->CheckCollsionAndGenerateContact(pair.first->ReturnShape(), pair.first->ReturnPos(),
			pair.second->ReturnShape(), pair.second->ReturnPos());
		//NARROW PHASE
	}

	for (auto go : GameObjectManager::GetInstance()->mGameObjects)
	{
		Body *pBody = static_cast<Body*>(go->GetComponent(BODY));
		if (pBody != nullptr && !pBody->mpOwner->isPaused)
		{
			pBody->Integrate(mFrameTime);//Semi Implicit Euler Integration
		}
	}

	//CONTACTS LOOP
	for (int i = 0; i < 10; i++)
	{
		for (auto c : CollisionManager::GetInstance()->mContacts)
		{
			if (c->colSide != NONE)
			{
				if (c->colSide == DOWN && !c->mBodies[0]->rRetrunAction()[InAir])
				{
					c->mBodies[0]->rReturnJumpC() = 0;
				}
				if (!c->mBodies[0]->collider && !c->mBodies[1]->collider)
				ResolveCollision(c);//Resolving collisions i.e applying impulses and friction
			}
		}
	}
	for (auto go : GameObjectManager::GetInstance()->mGameObjects)
	{
		Body *pBody = static_cast<Body*>(go->GetComponent(BODY));
		if (pBody != nullptr && !pBody->mpOwner->isPaused)
		{
			pBody->PosUpdate(mFrameTime);//Updating the position of all the bodies after applying impulses and changing the velocities
		}
	}

	for (auto c : CollisionManager::GetInstance()->mContacts)
	{
		if (c->mBodies[0]->mpOwner->isPaused || c->mBodies[1]->mpOwner->isPaused)
			continue;
		unsigned int temp = c->mBodies[0]->mpOwner->mHash | c->mBodies[1]->mpOwner->mHash;
		//Generating a hash value so that events could be triggered
		Event pEvent0(EventManager::GetInstance()->mEventMap[temp], c->mBodies[1]->mpOwner,c);
		c->mBodies[0]->mpOwner->HandleEvent(&pEvent0);

		Event pEvent1(EventManager::GetInstance()->mEventMap[temp], c->mBodies[0]->mpOwner,c);
		c->mBodies[1]->mpOwner->HandleEvent(&pEvent1);
	}

	for (auto c : CollisionManager::GetInstance()->mContacts)
	{
		if (!c->mBodies[0]->collider && !c->mBodies[1]->collider)
			PositionalCorrection(c);//Finally the positional correction 
	}
}

void PhysicsManager::Close()
{
	delete pObject;
}

void PhysicsManager::SetFPS(float _fps)
{
	mFPS = _fps;
	mFrameTime = 1.0f / mFPS;//Deterministic physics delta time for 60fps
}

float PhysicsManager::ReturnFrameTime()
{
	return mFrameTime;
}

