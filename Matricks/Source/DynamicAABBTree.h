/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#pragma once
#include"stdafx.h"
class Node
{
public:
	Node* mParent;
	Body* mClientData;	//This is the actual body stored in a Node
	Node* mLeft;		//Pointer to the left node		
	Node* mRight;		//Pointer to the right node
	ShapeAABB AABB;		//Nodes AABB data
	int height;			//Height of the node
	Node(void) : mParent(nullptr), mClientData(nullptr), mLeft(nullptr), mRight(nullptr) { height = 0; }

	bool IsLeaf();

	void UpdateAABB(float margin);// This will update the AABB of the nodes


	Node *GetSibling();//Return the pointer to the sibling
	
	void SetBranch(Node *n0, Node *n1);//Assignes the nodes as a branch

	void SetLeaf(Body *data);//Assigns the left and right pointer to null making it a leaf node

};

class DynamicTree : BroadPhase
{
public:
	DynamicTree(void)
		: m_root()
		, m_margin(25.0f) //this is the margin of the fattness of AABB (normal AABB in the game are 80 wide so increasing by 50 in total)
	{ }

	virtual void Add(Body *aabb);
	virtual void Update(void);
	virtual ColliderPairList &ComputePairs(void);
	virtual void Query(const Body &aabb, ColliderList &out) const;
	Node *m_root;
	void sync(Node* parent);
	ColliderPairList m_pairs;
private:
	typedef std::vector<Node *> NodeList;
	void UpdateNodeHelper(Node *node, NodeList &invalidNodes);
	void InsertNode(Node *node, Node **parent);
	void SelfQuery(Node* node);
	void SelfQuery(Node* A, Node* B);
	void SplitNodes(Node* A, Node* B);
	float m_margin;
private:
	NodeList m_invalidNodes;//These are the nodes that went out of their fat AABB
	ColliderList m_aabbs;//All the bodies in the Dynamic Tree
};