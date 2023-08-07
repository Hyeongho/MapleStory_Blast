#pragma once
#include "SceneComponent.h"
class CPrimitiveComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CPrimitiveComponent();
    CPrimitiveComponent(const CPrimitiveComponent& component);
    virtual ~CPrimitiveComponent();

protected:
    CSharedPtr<class CMesh> m_Mesh;
    std::vector<CSharedPtr<class CMaterial>> m_vecMaterial;

public:
    class CMaterial* GetMaterial(int Slot)
    {
        return m_vecMaterial[Slot];
    }

public:
    virtual bool SetMesh(const std::string& Name);
    virtual bool SetMesh(class CMesh* Mesh);
    virtual bool SetMesh(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    virtual bool SetMeshFullPath(const std::string& Name, const TCHAR* FullPath);
    void SetMaterial(int Slot, const std::string& Name);
    void SetMaterial(int Slot, class CMaterial* Material);
    void AddMaterial(const std::string& Name);
    void AddMaterial(class CMaterial* Material);
    void ClearMaterial();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CPrimitiveComponent* Clone() const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};
