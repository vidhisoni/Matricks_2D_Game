/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once
class Body;
class Shape
{
public:
	enum ShapeType
	{
		CIRCLE,
		AABB,
		NUM
	};
	Shape(ShapeType Type);
	virtual ~Shape();
	virtual glm::vec2 GetExtents() = 0;
	virtual bool TestPoint(float PointX, float PointY) = 0;
public:
	Body *mpOwnerBody;
	ShapeType mType;
};

//---------------CIRCLE SHAPE----------------------------------
class ShapeCircle :public Shape
{
public:
	ShapeCircle();
	~ShapeCircle();
	glm::vec2 GetExtents();
	bool TestPoint(float PointX, float PointY);
	void Initialize(glm::vec2 _Extent, glm::vec2 _pos);
	glm::vec2 ReturnPos();
	glm::vec2 ReturnExtent();
	float ReturnRadius();
	glm::vec2& rReturnPos();
private:
	float mRadius;
	glm::vec2 Extent;
	glm::vec2 Position;
};
//---------------CIRCLE SHAPE----------------------------------


//-----------------AABB SHAPE--------------------------------
class ShapeAABB :public Shape
{
public:
	ShapeAABB();
	~ShapeAABB();
	glm::vec2 GetExtents();
	bool TestPoint(float PointX, float PointY);
	float SurfaceArea();
	ShapeAABB Union(ShapeAABB B);
	bool Contains(ShapeAABB check);
	glm::vec2 GetWorldMin();
	glm::vec2 GetWorldMax();
	void Initialize(glm::vec2 _Extent, glm::vec2 _pos);
	glm::vec2 ReturnPos();
	glm::vec2 ReturnExtent();
	glm::vec2& rReturnPos();
private:
	glm::vec2 Extent;//Minimum and maximum points in the world coordinate
	glm::vec2 Position;
};
//-----------------AABB SHAPE--------------------------------

enum CollisionSide {
	NONE = -1,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//************CONTACT************************ MANIFOLD 
class Contact
{
public:
	Contact();
	~Contact();

public:
	Body *mBodies[2];
	CollisionSide colSide;
	glm::vec2 differenceVector;
	glm::vec2 ContactNormal;
	float penetration;
};

//==========Collision Manager=================================================
class CollisionManager
{
	static CollisionManager* pObject;
	CollisionManager();
public:
	~CollisionManager();
	static CollisionManager* CreateInstance();
	inline static CollisionManager* GetInstance() { return pObject; }
	void Init();
	void Close();

	void Reset();

	bool CheckCollsionAndGenerateContact(Shape *pShape1, glm::vec2 Pos1,
		Shape *pShape2, glm::vec2 Pos2);

	int PointToRect(glm::vec2 point, glm::vec2 RectCenter, glm::vec2 WidthHeightScale);

public:
	std::list<Contact *>mContacts;
	bool(*CollisionFunctions[Shape::NUM][Shape::NUM])(Shape *pShape1, glm::vec2 Pos1,
		Shape *pShape2, glm::vec2 Pos2,
		std::list<Contact*>&Contacts);
};

//==========Collision Manager=================================================
