/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Shantanu Chauhan
- End Header --------------------------------------------------------*/
#include"stdafx.h"
#include<algorithm>

//NODE START
void Node::SetLeaf(Body * data)//Attach the objects body to the node
{         
	this->mClientData = data;
	//data->userData = this;(TODO:This will be used for deletion of the node) Deletion is not done yet
	height = 0;
	mRight = nullptr;
	mLeft = nullptr;

}

void Node::SetBranch(Node * n0, Node * n1)//This simply sets the 2 given nodes as childs of the node that this is being called upon
{
	n0->mParent = this;
	n1->mParent = this;
	mLeft = n0;
	mRight = n1;
}

Node * Node::GetSibling()
{
	return
		this == this->mParent->mLeft
		? this->mParent->mRight
		: this->mParent->mLeft;
}

bool Node::IsLeaf()
{
	return (mRight == nullptr && mLeft == nullptr);
}

void Node::UpdateAABB(float margin)//This will combine two nodes or fatten up a leafnode aabb
{
	if (IsLeaf())//If leaf fatten it up
	{
		const glm::vec2 marginVec(margin, margin);
		if (mClientData->ReturnShape()->mType == Shape::ShapeType::CIRCLE)
		{
			ShapeCircle* clientshape = static_cast<ShapeCircle*>(mClientData->ReturnShape());
			AABB.Initialize(clientshape->ReturnExtent() + marginVec, clientshape->ReturnPos());
		}
		else
		{
			ShapeAABB* clientshape = static_cast<ShapeAABB*>(mClientData->ReturnShape());
			AABB.Initialize(clientshape->ReturnExtent() + marginVec, clientshape->ReturnPos());
		}
	}
	else//Ohterwise combine the two AABB and update the height
	{
		AABB = mRight->AABB.Union(mLeft->AABB);
		//Update the height of the Node
		this->height = 1 + (this->mLeft->height > this->mRight->height
			? this->mLeft->height : this->mRight->height);
	}
}
//NODE END

//Dynamic AABB Tree below
void DynamicTree::Add(Body * aabb)
{
	if (m_root)//Tree is not empty
	{
		Node *node = new Node();
		node->SetLeaf(aabb);
		node->UpdateAABB(m_margin);
		InsertNode(node, &m_root);
	}
	else//Tree is empty so create the root node
	{
		m_root = new Node();
		m_root->SetLeaf(aabb);
		m_root->UpdateAABB(m_margin);
	}
}


void DynamicTree::InsertNode(Node * node, Node ** parent)//Inserting the node inside the Tree
{
	Node *p = *parent;
	if (p->IsLeaf())
	{
		// Parent doesnt have any children so create a new node which has both the nodes as children
		Node *newParent = new Node();
		newParent->mParent = p->mParent;
		newParent->SetBranch(node, p);
		*parent = newParent;
	}
	else
	{
		// Parent is branch, compute surface area differences and add it to the one which increases the area the least
		ShapeAABB* aabb0 = static_cast<ShapeAABB*>(&(p->mLeft->AABB));
		ShapeAABB* aabb1 = static_cast<ShapeAABB*>(&(p->mRight->AABB));

		//Surface Area difference checker
		const float SurfaceDiff0 =
			aabb0->Union(node->AABB).SurfaceArea() - aabb0->SurfaceArea();
		const float SurfaceDiff1 =
			aabb1->Union(node->AABB).SurfaceArea() - aabb1->SurfaceArea();
		//Adding to the side which changes the surfacearea the least
		if (SurfaceDiff0 < SurfaceDiff1)
		{
			InsertNode(node, &p->mLeft);
		}
		else
		{
			InsertNode(node, &p->mRight);
		}
		//Surface Area difference checker
	}
	sync(*parent);//This syncs the whole tree(i.e. update the tree from bottom up, the height and areas also)
}


void DynamicTree::sync(Node* parent)//To update the aabb of the nodes and to update the height while propogating up
{
	Node* p = parent;
	while (p != nullptr) {
		Node* l = parent->mLeft;
		Node* r = parent->mRight;
		p->height = 1 + std::max(l->height, r->height);
		p->AABB = l->AABB.Union(r->AABB);
		p = p->mParent;
	}
}

void DynamicTree::Update(void)//To rebalance/reinsert the moving nodes in the tree
{
	if (m_root)
	{
		if (m_root->IsLeaf())
			m_root->UpdateAABB(m_margin);
		else
		{
			// Invalid nodes are the nodes whose cliendata/objects body has moved out of the fattened AABB
			m_invalidNodes.clear();
			UpdateNodeHelper(m_root, m_invalidNodes);

			// re-insert all invalid nodes
			for (Node *node : m_invalidNodes)
			{
				Node *parent = node->mParent;
				Node *sibling = node->GetSibling();
				Node **parentLink =
					parent->mParent
					? (parent == parent->mParent->mLeft
						? &parent->mParent->mLeft
						: &parent->mParent->mRight)
					: &m_root;

				sibling->mParent =
					parent->mParent
					? parent->mParent
					: nullptr;
				*parentLink = sibling;
				delete parent;
				sync(sibling->mParent);//This is important as the treenow has a whole in it, so update it
				node->UpdateAABB(m_margin);
				InsertNode(node, &m_root);//After the whole was fixed now insert from the top
			}
			m_invalidNodes.clear();
		}
	}
}

ColliderPairList & DynamicTree::ComputePairs(void)//To find all the Leafs that are colliding and storing them in m_pairs
{
	m_pairs.clear();
	if (!m_root || m_root->IsLeaf())//Nothing is there just get out
		return m_pairs;
	else
		SelfQuery(m_root);//Let the recursion games commence
	return m_pairs;
}

void DynamicTree::Query(const Body & aabb, ColliderList & out) const//TODO: Have to implement this so that other trees AABB can query a tree
{
	//Actually wanted to do this but couldnt(NEXT GAM THIS WILL BE DONE for 3D -promise!)
}

void DynamicTree::UpdateNodeHelper(Node * node, NodeList & invalidNodes)//This is to find the invalid nodes
{
	if (node->IsLeaf())
	{
		//Checking if the body got out of the fattened AABB
		ShapeAABB* temp = static_cast<ShapeAABB*>(node->mClientData->ReturnShape());
		if (temp == nullptr)
			return;
		if (!node->AABB.Contains(*temp))//It did, so push it to the invalid list and reinsert in the update
			invalidNodes.push_back(node);
	}
	else
	{
		UpdateNodeHelper(node->mLeft, invalidNodes);
		UpdateNodeHelper(node->mRight, invalidNodes);
	}
}

void DynamicTree::SelfQuery(Node * node)
{
	if (node->IsLeaf())
		return;
	SelfQuery(node->mLeft, node->mRight);//Recursion games
	SelfQuery(node->mLeft);				 //Recursion games
	SelfQuery(node->mRight);			 //Recursion games
}
bool Collidesss(ShapeAABB A, ShapeAABB B)
//This is just for collision checking between leaf nodes fat aabb
{
	glm::vec2 Acurrmin = A.GetWorldMin();
	glm::vec2 Bcurrmin = B.GetWorldMin();

	glm::vec2 Acurrmax = A.GetWorldMax();
	glm::vec2 Bcurrmax = B.GetWorldMax();
	if (Acurrmin.x > Bcurrmax.x || Acurrmax.x < Bcurrmin.x || Acurrmin.y > Bcurrmax.y || Acurrmax.y < Bcurrmin.y)
		return false;
	else
		return true;
}
void DynamicTree::SelfQuery(Node * A, Node * B)//Final stage of the recursion games
{
	if (A->IsLeaf() && B->IsLeaf())
	{
		bool bPush = true;
		if (A->mClientData == B->mClientData)
			bPush = false;
		if (A->mClientData->mpOwner->isPaused)
			bPush = false;
		if (B->mClientData->mpOwner->isPaused)
			bPush = false;
		if (A->mClientData->ReturnMass() == 0.0 && B->mClientData->ReturnMass() == 0.0)
			bPush = false;
		if (bPush)
		{
			if (Collidesss(A->AABB, B->AABB))//Leafs fat aabbs are being checked for collision
			{
				if(B->mClientData->mpOwner->GetType() == PLAYER)
				m_pairs.push_back(
					std::make_pair(B->mClientData, A->mClientData));//Make pairs and push
				else
					m_pairs.push_back(
						std::make_pair(A->mClientData, B->mClientData));
			}
		}
	}
	else if (A->IsLeaf() && !B->IsLeaf())//A is leaf B is internal so run it again with (A , B leafs)
	{
		SelfQuery(A, B->mLeft);
		SelfQuery(A, B->mRight);
	}
	else if (!A->IsLeaf() && B->IsLeaf())//B is leaf A is internal so run it again with (A leafs, B )
	{
		SelfQuery(A->mLeft, B);
		SelfQuery(A->mRight, B);
	}
	else
		SplitNodes(A, B);//Both are internal
}


void DynamicTree::SplitNodes(Node * A, Node * B)
{
	SelfQuery(A->mLeft, B->mLeft);  //Both are internal so recursively call with all the child combinations
	SelfQuery(A->mLeft, B->mRight);	//Both are internal so recursively call with all the child combinations
	SelfQuery(A->mRight, B->mLeft);	//Both are internal so recursively call with all the child combinations
	SelfQuery(A->mRight, B->mRight);//Both are internal so recursively call with all the child combinations
}


