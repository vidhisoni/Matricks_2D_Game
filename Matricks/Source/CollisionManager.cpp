/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#include "stdafx.h"

CollisionManager * CollisionManager::pObject = nullptr;

float clamp(float value, float min, float max) {
	return std::max(min, std::min(max, value));
}

CollisionManager* CollisionManager::CreateInstance()
{
	pObject = new CollisionManager();
	return pObject;
}

Shape::Shape(ShapeType Type)
{
	mType = Type;
	mpOwnerBody = nullptr;
}

Shape::~Shape()
{
	mpOwnerBody = nullptr;
}
//SHAPECIRCLE START
ShapeCircle::ShapeCircle() :Shape(CIRCLE)
{
	mRadius = 0.0f;
	Position = glm::vec2(0.0f);
	Extent = glm::vec2(0.0f);

}
ShapeCircle::~ShapeCircle()
{
	mRadius = 0.0f;
	Position = glm::vec2(0.0f);
	Extent = glm::vec2(0.0f);
}
glm::vec2 ShapeCircle::GetExtents()
{
	return glm::vec2(mRadius, mRadius);
}
bool ShapeCircle::TestPoint(float PointX, float PointY)
{
	glm::vec2 VectorToPoint = glm::vec2(PointX, PointY);
	VectorToPoint = Position - VectorToPoint;
	if (glm::length(VectorToPoint) <= mRadius)
		return true;
	else
		return false;
}

void ShapeCircle::Initialize(glm::vec2 _Extent, glm::vec2 _pos)
{
	Extent = _Extent;//Setting the extent for the dynamic AABB
	Position = _pos;//Kepping the position of the Shapes center in world coordinate
	mRadius = Extent.x / 2.0f;//Radius is simple half of the extent
}

glm::vec2 ShapeCircle::ReturnPos()
{
	return Position;
}

glm::vec2 ShapeCircle::ReturnExtent()
{
	return Extent;
}

float ShapeCircle::ReturnRadius()
{
	return mRadius;
}

glm::vec2 & ShapeCircle::rReturnPos()
{
	return Position;
}

//SHAPECIRCLE END

//ShapeAABB start
ShapeAABB::ShapeAABB() :Shape(AABB)
{
	Extent = Position = glm::vec2(0.0f);
}
ShapeAABB::~ShapeAABB()
{
	Extent = Position = glm::vec2(0.0f);
}

glm::vec2 ShapeAABB::GetExtents()
{
	return Extent / 2.0f;
}

void ShapeAABB::Initialize(glm::vec2 _Extent, glm::vec2 _pos)
{
	Extent = _Extent;
	Position = _pos;
}

glm::vec2 ShapeAABB::ReturnPos()
{
	return Position;
}

glm::vec2 ShapeAABB::ReturnExtent()
{
	return Extent;
}

glm::vec2 & ShapeAABB::rReturnPos()
{
	return Position;
}

glm::vec2 ShapeAABB::GetWorldMin()
{
	return  Position - (Extent / 2.0f);
}

glm::vec2 ShapeAABB::GetWorldMax()
{
	return Position + (Extent / 2.0f);
}

bool ShapeAABB::TestPoint(float PointX, float PointY)
{
	if (PointX > GetWorldMin().x &&PointY > GetWorldMin().y &&PointX < GetWorldMax().x &&PointY < GetWorldMax().y)
		return true;
	else
		return false;
}

float ShapeAABB::SurfaceArea()
{
	float surfaceArea;
	float length, height;
	length = Extent.x;
	height = Extent.y;
	surfaceArea = length * height;
	return surfaceArea;
}

ShapeAABB ShapeAABB::Union(ShapeAABB B)//Combining the 2 aabb and returning a bigger one
{
	ShapeAABB merge;
	glm::vec2 Amax = this->GetWorldMax();
	glm::vec2 Bmax = B.GetWorldMax();
	glm::vec2 Amin = this->GetWorldMin();
	glm::vec2 Bmin = B.GetWorldMin();

	glm::vec2 min, max;
	min.x = Amin.x > Bmin.x ? Bmin.x : Amin.x;
	min.y = Amin.y > Bmin.y ? Bmin.y : Amin.y;


	max.x = Amax.x < Bmax.x ? Bmax.x : Amax.x;
	max.y = Amax.y < Bmax.y ? Bmax.y : Amax.y;


	merge.Extent = (max - min);
	merge.Position = (max + min) / 2.0f;
	return merge;
}
bool ShapeAABB::Contains(ShapeAABB check)//Checking  the given AABB is contained in the current AABB
{
	bool yes = true;
	glm::vec2 checkmax, checkmin;
	glm::vec2 currentmax, currentmin;
	checkmax = check.GetWorldMax();
	checkmin = check.GetWorldMin();
	currentmax = this->GetWorldMax();
	currentmin = this->GetWorldMin();

	if (checkmin.x > currentmin.x &&checkmin.x < currentmax.x &&
		checkmax.x > currentmin.x &&checkmax.x < currentmax.x
		);
	else return (false);

	if (checkmin.y > currentmin.y &&checkmin.y < currentmax.y &&
		checkmax.y > currentmin.y &&checkmax.y < currentmax.y
		);
	else return (false);

	return yes;
}
//ShapeAABB end

bool CheckCollisionAABBToAABB(Shape *pShape1, glm::vec2 Pos1,
	Shape *pShape2, glm::vec2 Pos2, std::list<Contact*>&Contacts);
bool CheckCollisionAABBToCIRCLE(Shape *pShape1, glm::vec2 Pos1,
	Shape *pShape2, glm::vec2 Pos2, std::list<Contact*>&Contacts);
bool CheckCollisionCIRCLEToAABB(Shape *pShape1, glm::vec2 Pos1,
	Shape *pShape2, glm::vec2 Pos2, std::list<Contact*>&Contacts);
bool CheckCollisionCIRCLEToCIRCLE(Shape *pShape1, glm::vec2 Pos1,
	Shape *pShape2, glm::vec2 Pos2, std::list<Contact*>&Contacts);

CollisionManager::CollisionManager()
{
	CollisionFunctions[Shape::AABB][Shape::AABB] = CheckCollisionAABBToAABB;
	CollisionFunctions[Shape::AABB][Shape::CIRCLE] = CheckCollisionAABBToCIRCLE;
	CollisionFunctions[Shape::CIRCLE][Shape::AABB] = CheckCollisionCIRCLEToAABB;
	CollisionFunctions[Shape::CIRCLE][Shape::CIRCLE] = CheckCollisionCIRCLEToCIRCLE;
}
CollisionManager::~CollisionManager()
{
	CollisionFunctions[Shape::AABB][Shape::AABB] = nullptr;
	CollisionFunctions[Shape::AABB][Shape::CIRCLE] = nullptr;
	CollisionFunctions[Shape::CIRCLE][Shape::AABB] = nullptr;
	CollisionFunctions[Shape::CIRCLE][Shape::CIRCLE] = nullptr;
}

void CollisionManager::Init()
{
	pObject = new CollisionManager();
}

void CollisionManager::Close()
{
	Reset();
	delete pObject;
}

void CollisionManager::Reset()
{
	for (auto c : mContacts)
		delete c;
	mContacts.clear();
}

int StaticRectToStaticRect(glm::vec2 *pRect0, float Width0, float Height0, glm::vec2 *pRect1, float Width1, float Height1)
{

	float rect1_xMin, rect1_xMax, rect1_yMin, rect1_yMax, rect2_xMin, rect2_xMax, rect2_yMin, rect2_yMax;
	rect1_xMin = pRect0->x - .5f*Width0;
	rect1_xMax = pRect0->x + .5f*Width0;
	rect1_yMin = pRect0->y - .5f*Height0;
	rect1_yMax = pRect0->y + .5f*Height0;

	rect2_xMin = pRect1->x - .5f*Width1;
	rect2_xMax = pRect1->x + .5f*Width1;
	rect2_yMin = pRect1->y - .5f*Height1;
	rect2_yMax = pRect1->y + .5f*Height1;

	if (rect1_xMin > rect2_xMax || rect1_xMax < rect2_xMin || rect1_yMax<rect2_yMin || rect1_yMin>rect2_yMax)
		return 0;
	else
		return 1;

}

bool CollisionManager::CheckCollsionAndGenerateContact(Shape * pShape1, glm::vec2 Pos1, Shape * pShape2, glm::vec2 Pos2)
{
	if (pShape1 != NULL && pShape2 != NULL)
		return CollisionFunctions[pShape1->mType][pShape2->mType](pShape1, Pos1, pShape2, Pos2, mContacts);
	else
		return false;
}

int CollisionManager::PointToRect(glm::vec2 point, glm::vec2 RectCenter, glm::vec2 WidthHeightScale)
{
	if ((point.x >= RectCenter.x - WidthHeightScale.x / 2.0f&&point.x <= RectCenter.x + WidthHeightScale.x / 2.0f)
		&& (point.y >= RectCenter.y - WidthHeightScale.y / 2.0f&&point.y <= RectCenter.y + WidthHeightScale.y / 2.0f))
		return 1;
	else return 0;
}

//=============Collision Checks===========

bool CheckCollisionAABBToAABB(Shape *pShape1, glm::vec2 Pos1,
	Shape *pShape2, glm::vec2 Pos2, std::list<Contact*>&Contacts)
{
	ShapeAABB *BOX1 = static_cast<ShapeAABB*>(pShape1);
	ShapeAABB *BOX2 = static_cast<ShapeAABB*>(pShape2);
	bool check = static_cast<bool>(StaticRectToStaticRect(&Pos1, BOX1->ReturnExtent().x, BOX1->ReturnExtent().y,
		&Pos2, BOX2->ReturnExtent().x, BOX2->ReturnExtent().y));
	if (check)//Now generating Contact Manifold
	{
		Contact *pNewContact = new Contact();
		pNewContact->mBodies[0] = pShape1->mpOwnerBody;
		pNewContact->mBodies[1] = pShape2->mpOwnerBody;


		glm::vec2 normal;
		normal = pNewContact->mBodies[1]->ReturnPos() - pNewContact->mBodies[0]->ReturnPos();//PosB-PosA
		float xExtentA, xExtentB, yExtentA, yExtentB;

		xExtentA = BOX1->GetExtents().x;
		xExtentB = BOX2->GetExtents().x;

		float xOverlap = xExtentA + xExtentB - abs(normal.x);
		if (xOverlap > 0.0f)
		{
			yExtentA = BOX1->GetExtents().y;
			yExtentB = BOX2->GetExtents().y;
			float yOverlap = yExtentA + yExtentB - abs(normal.y);
			if (yOverlap > 0.0f)
			{
				if (xOverlap < yOverlap)//X axis least penetration
				{
					if (normal.x < 0.0f)//LEFT OR RIGHT?
					{
						pNewContact->ContactNormal = glm::vec2(-1.0f, 0.0f);
						pNewContact->colSide = LEFT;
					}
					else
					{
						pNewContact->ContactNormal = glm::vec2(1.0f, 0.0f);
						pNewContact->colSide = RIGHT;
					}
					pNewContact->penetration = xOverlap;
				}
				else//Y axis least penetration
				{
					if (normal.y < 0.0f)//Check if its UP or DOWN
					{
						pNewContact->ContactNormal = glm::vec2(0.0f, -1.0f);
						pNewContact->colSide = DOWN;
						if (pNewContact->mBodies[0]->rRetrunAction()[Jumping] && !pNewContact->mBodies[1]->collider)
							pNewContact->mBodies[0]->rRetrunAction()[Jumping] = false;
						else
						{
							if (!pNewContact->mBodies[1]->collider)
							{
								pNewContact->mBodies[0]->rRetrunAction()[OnGround] = true;
								pNewContact->mBodies[0]->rRetrunAction()[InAir] = false;
							}
						}
					}
					else
					{
						pNewContact->ContactNormal = glm::vec2(0.0f, 1.0f);
						pNewContact->colSide = UP;
					}
					pNewContact->penetration = yOverlap;
				}
			}

		}
		//Push the contact to the manifold
		Contacts.push_back(pNewContact);
		return check;
	}
	else
		return check;
}

bool CheckCollisionAABBToCIRCLE(Shape * pShape1, glm::vec2 Pos1, Shape * pShape2, glm::vec2 Pos2, std::list<Contact*>& Contacts)
{
	// Vector from A to B
	ShapeAABB *Box = static_cast<ShapeAABB*>(pShape1);
	ShapeCircle *Circle = static_cast<ShapeCircle*>(pShape2);
	glm::vec2 n = Circle->ReturnPos() - Box->ReturnPos();

	//Findind the closest point btw A and B
	glm::vec2 closest = n;

	//half extents
	float x_extent = Box->GetExtents().x;
	float y_extent = Box->GetExtents().y;

	// Clamping to the edges of the AABB
	closest.x = glm::clamp(closest.x, -x_extent, x_extent);
	closest.y = glm::clamp(closest.y, -y_extent, y_extent);

	bool inside = false;

	//Deep penetration clamp to the edge of AABB
	if (n == closest)
	{
		inside = true;

		if (abs(n.x) > abs(n.y))
		{
			if (closest.x > 0)
				closest.x = x_extent;
			else
				closest.x = -x_extent;
		}
		else
		{
			if (closest.y > 0)
				closest.y = y_extent;
			else
				closest.y = -y_extent;
		}
	}
	glm::vec2 normal = n - closest;
	float d = glm::length(normal);
	float r = Circle->ReturnRadius();
	if (d > r && !inside)
		return false;
	Contact *pNewContact = new Contact();//Now generating a contact
	pNewContact->mBodies[0] = pShape1->mpOwnerBody;
	pNewContact->mBodies[1] = pShape2->mpOwnerBody;
	normal = glm::normalize(normal);
	if (inside)
	{
		pNewContact->ContactNormal = -normal;
		pNewContact->penetration = r - d;
	}
	else
	{
		pNewContact->ContactNormal = normal;
		pNewContact->penetration = r - d;
	}
	if (pNewContact->ContactNormal == glm::vec2(-1.0f, 0.0f))
		pNewContact->colSide = LEFT;
	else
		if (pNewContact->ContactNormal == glm::vec2(1.0f, 0.0f))
			pNewContact->colSide = RIGHT;
		else if (pNewContact->ContactNormal == glm::vec2(0.0f, -1.0f))
			pNewContact->colSide = DOWN;
		else if (pNewContact->ContactNormal == glm::vec2(0.0f, 1.0f))
			pNewContact->colSide = UP;
	Contacts.push_back(pNewContact);//Pushing the contact to the manifold
	return true;
}

bool CheckCollisionCIRCLEToAABB(Shape * pShape1, glm::vec2 Pos1, Shape * pShape2, glm::vec2 Pos2, std::list<Contact*>& Contacts)
{
	// Vector from A to B
	ShapeAABB *Box = static_cast<ShapeAABB*>(pShape2);
	ShapeCircle *Circle = static_cast<ShapeCircle*>(pShape1);
	glm::vec2 n = Circle->ReturnPos() - Box->ReturnPos();

	glm::vec2 closest = n;

	float x_extent = Box->GetExtents().x;
	float y_extent = Box->GetExtents().y;

	closest.x = glm::clamp(closest.x, -x_extent, x_extent);
	closest.y = glm::clamp(closest.y, -y_extent, y_extent);

	bool inside = false;

	if (n == closest)
	{
		inside = true;

		if (abs(n.x) > abs(n.y))
		{
			if (closest.x > 0)
				closest.x = x_extent;
			else
				closest.x = -x_extent;
		}
		else
		{
			if (closest.y > 0)
				closest.y = y_extent;
			else
				closest.y = -y_extent;
		}
	}

	glm::vec2 normal = n - closest;
	float d = glm::length(normal);
	float r = Circle->ReturnRadius();
	if (d > r && !inside)
		return false;
	Contact *pNewContact = new Contact();
	pNewContact->mBodies[0] = pShape1->mpOwnerBody;
	pNewContact->mBodies[1] = pShape2->mpOwnerBody;
	normal = glm::normalize(normal);
	if (inside)
	{
		pNewContact->ContactNormal = -normal;
		pNewContact->penetration = r - d;
	}
	else
	{
		pNewContact->ContactNormal = normal;
		pNewContact->penetration = r - d;
	}
	if (pNewContact->ContactNormal == glm::vec2(-1.0f, 0.0f))
		pNewContact->colSide = LEFT;
	else
		if (pNewContact->ContactNormal == glm::vec2(1.0f, 0.0f))
			pNewContact->colSide = RIGHT;
		else if (pNewContact->ContactNormal == glm::vec2(0.0f, -1.0f))
			pNewContact->colSide = DOWN;
		else if (pNewContact->ContactNormal == glm::vec2(0.0f, 1.0f))
			pNewContact->colSide = UP;
	Contacts.push_back(pNewContact);
	return true;
}

bool CheckCollisionCIRCLEToCIRCLE(Shape * pShape1, glm::vec2 Pos1, Shape * pShape2, glm::vec2 Pos2, std::list<Contact*>& Contacts)
{
	ShapeCircle *Circle1 = static_cast<ShapeCircle*>(pShape2);
	ShapeCircle *Circle2 = static_cast<ShapeCircle*>(pShape1);
	bool check = false;
	glm::vec2 n = Pos2 - Pos1;

	float r = Circle1->ReturnRadius() + Circle2->ReturnRadius();
	float d = glm::length(n);
	if (d > r)
		return false;
	Contact *pNewContact = new Contact();
	pNewContact->mBodies[0] = pShape1->mpOwnerBody;
	pNewContact->mBodies[1] = pShape2->mpOwnerBody;
	if (d != 0)
	{
		pNewContact->penetration = r - d;
		pNewContact->ContactNormal = n / d;
		check = true;
	}
	else
	{

		pNewContact->penetration = Circle1->ReturnRadius();
		pNewContact->ContactNormal = glm::vec2(1.0f, 0.0f);
		check = true;
	}
	if (pNewContact->ContactNormal == glm::vec2(-1.0f, 0.0f))
		pNewContact->colSide = LEFT;
	else
		if (pNewContact->ContactNormal == glm::vec2(1.0f, 0.0f))
			pNewContact->colSide = RIGHT;
		else
			if (pNewContact->ContactNormal == glm::vec2(0.0f, -1.0f))
				pNewContact->colSide = DOWN;
			else
				if (pNewContact->ContactNormal == glm::vec2(0.0f, 1.0f))
					pNewContact->colSide = UP;
	Contacts.push_back(pNewContact);
	return check;
}

Contact::Contact()
{
	mBodies[0] = nullptr;
	mBodies[1] = nullptr;
	differenceVector = glm::vec2(0.0f);
	colSide = CollisionSide::NONE;
	penetration = 0.0f;
	ContactNormal = glm::vec2(0.0f);
}

Contact::~Contact()
{
	mBodies[0] = nullptr;
	mBodies[1] = nullptr;
	differenceVector = glm::vec2(0.0f);
	colSide = CollisionSide::NONE;
	penetration = 0.0f;
	ContactNormal = glm::vec2(0.0f);
}

