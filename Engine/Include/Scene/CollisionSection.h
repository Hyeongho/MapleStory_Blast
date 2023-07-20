#pragma once

#include "../EngineInfo.h"

class CCollisionSection
{
	friend class CSceneCollision;

private:
	CCollisionSection();
	~CCollisionSection();

private:
	Vector2 m_Min;
	Vector2 m_Max;
	int m_IndexX;
	int m_IndexY;
	int m_Index;
	std::list<std::shared_ptr<class CCollider2D>> m_ColliderList;

public:
	void AddCollider(class CCollider2D* Collider);
	void Collision(float DeltaTime);
	class CCollider* CollisionMouse(const Vector2& MouseWorldPos, float DeltaTime);

private:
	static bool ColliderSort(class CCollider2D* Src, class CCollider2D* Dest);
};

