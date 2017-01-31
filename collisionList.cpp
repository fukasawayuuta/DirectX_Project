#include "main.h"  
#include "collision.h"
#include "collisionList.h"

CCollisionList::CCollisionList()
{
}


CCollisionList::~CCollisionList()
{
}

void CCollisionList::Init(void)
{
}

void CCollisionList::Uninit(void)
{
	m_CollisionList.clear();
}

void CCollisionList::AddList(CCollision * collision)
{
	m_CollisionList.push_back(collision);
}
