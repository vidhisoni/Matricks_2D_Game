/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once
#include"BroadPhase.h"

class NSquared : public BroadPhase
{
public:

	virtual void Add(Body *aabb);
	

	virtual void Update(void)
	{
		//Nothing happens in the nsquared for the update as all the objects are inside it already
	}

	virtual ColliderPairList &ComputePairs(void);
	virtual void Query(const Body &aabb, ColliderList &out) const {};
	ColliderList m_aabbs;

	ColliderPairList m_pairs;
private:
};