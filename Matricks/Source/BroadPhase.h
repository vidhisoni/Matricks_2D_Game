/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once
#include"stdafx.h"
#include<vector>
class ShapeAABB;
class Body;

typedef std::pair<Body *, Body *> ColliderPair;
typedef std::list<ColliderPair> ColliderPairList;

class BroadPhase
{
public:

	// Pass the body to add it to the BroadPhase
	virtual void Add(Body *aabb) = 0;

	//Update the Bodies in the BroadPhase
	virtual void Update(void) = 0;

	// Will return the possibly colliding pairs
	virtual const ColliderPairList &ComputePairs(void) = 0;

	// returns a list of colliders whose bodies collide 
	// with a query body(TODO)
	typedef std::vector<Body *> ColliderList;
	virtual void
		Query(const Body &aabb, ColliderList &output) const = 0;
};