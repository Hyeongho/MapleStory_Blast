#pragma once

#include "SceneComponent.h"

class CCollider :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CScene;
    friend class CCollisionManager;

protected:
    CCollider();
    CCollider(const CCollider& component);
    virtual ~CCollider();

protected:
    std::shared_ptr<class CMesh> m_Mesh;
    std::shared_ptr<class CShader> m_Shader;
    Vector4 m_Color;
    ECollider_Type m_ColliderType;
    Vector3 m_Min;
    Vector3 m_Max;
    CollisionResult m_Result;
    CollisionResult m_MouseResult;
    std::list<CCollider*> m_PrevCollisionList;
    std::list<int> m_CurrentSectionList;
    CollisionProfile* m_Profile;
    Vector3 m_HitPoint;
    bool m_MouseCollision;


    std::function<void(const CollisionResult&)> m_CollisionCallback[(int)ECollision_Result::Max];
    std::function<void(const CollisionResult&)> m_CollisionMouseCallback[(int)ECollision_Result::Max];

public:
    const Vector3& GetMin() const
    {
        return m_Min;
    }

    const Vector3& GetMax() const
    {
        return m_Max;
    }

    ECollider_Type GetColliderType()    const
    {
        return m_ColliderType;
    }

    CollisionProfile* GetCollisionProfile()  const
    {
        return m_Profile;
    }

public:
    void AddSectionIndex(int Index)
    {
        m_CurrentSectionList.push_back(Index);
    }

    void AddPrevCollisionCollider(CCollider* Collider)
    {
        m_PrevCollisionList.push_back(Collider);
    }

    void DeletePrevCollisionCollider(CCollider* Collider);

    bool CheckPrevCollision(CCollider* Collider);

    void CheckPrevCollisionColliderSection();

    void SendPrevCollisionEnd();

    void CallCollisionCallback(ECollision_Result Type);
    void CallCollisionMouseCallback(ECollision_Result Type);
    void ClearFrame();
    int CheckOverlapSection(CCollider* Dest);
    void SetCollisionProfile(const std::string& Name);

    void SetCollisionResultDest(CCollider* Dest)
    {
        m_Result.Dest = Dest;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CCollider* Clone()  const = 0;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CCollider* Dest) = 0;

public:
    template <typename T>
    void SetCollisionCallback(ECollision_Result Type, T* Obj, void(T::* Func)(const CollisionResult&))
    {
        m_CollisionCallback[(int)Type] = std::bind(Func, Obj, std::placeholders::_1);
    }

    template <typename T>
    void SetCollisionMouseCallback(ECollision_Result Type, T* Obj, void(T::* Func)(const CollisionResult&))
    {
        m_CollisionMouseCallback[(int)Type] = std::bind(Func, Obj, std::placeholders::_1);
    }
};

