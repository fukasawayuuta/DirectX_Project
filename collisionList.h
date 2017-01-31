#pragma once
class CCollision;

class CCollisionList
{
public:
	CCollisionList();
	~CCollisionList();

	void Init(void);
	void Uninit(void);

	void AddList(CCollision *collision);

	std::list<CCollision *> *GetCollision(void) { return &m_CollisionList; }

protected:
	std::list<CCollision *> m_CollisionList;
};

