/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#include"stdafx.h"
bool Collides(Body* A, Body* B);

void NSquared::Add(Body * aabb)
{
	m_aabbs.push_back(aabb);
}

ColliderPairList & NSquared::ComputePairs(void)
{
	m_pairs.clear();//clearing the pairs

	auto end = m_aabbs.end();
	for (auto i = m_aabbs.begin(); i != end; ++i)
	{
		auto jStart = i;
		for (auto j = ++jStart; j != end; ++j)
		{
			Body *colliderA = *i;
			Body *colliderB = *j;

			// skip same-body collision
			if (colliderA == colliderB)
				continue;
			if (colliderA->mpOwner->isPaused)
				continue;
			if (colliderB->mpOwner->isPaused)
				continue;
			if (colliderA->ReturnMass() == 0.0 && colliderB->ReturnMass() == 0.0)
				continue;
			if (colliderA->ReturnShape() == NULL || colliderB->ReturnShape() == NULL)
				continue;
			if (Collides(colliderA, colliderB))
			{
				m_pairs.push_back(
					std::make_pair(colliderA, colliderB));//Collision happens add the pairs
			}
		}
	}
	return m_pairs;
}

bool Collides(Body* A, Body* B)
{
	ShapeAABB* pA = static_cast<ShapeAABB*>(A->ReturnShape());
	ShapeAABB* pB = static_cast<ShapeAABB*>(B->ReturnShape());

	glm::vec2 Acurrmin = pA->GetWorldMin();
	glm::vec2 Bcurrmin = pB->GetWorldMin();

	glm::vec2 Acurrmax = pA->GetWorldMax();
	glm::vec2 Bcurrmax = pB->GetWorldMax();
	if (Acurrmin.x > Bcurrmax.x || Acurrmax.x < Bcurrmin.x || Acurrmin.y > Bcurrmax.y || Acurrmax.y < Bcurrmin.y )
		return false;
	else
		return true;
}