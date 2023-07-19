#pragma once

#include "PrimitiveComponent.h"
#include "../Animation/Animation.h"

class CSpriteComponent :
    public CPrimitiveComponent
{
    friend class CGameObject;
    friend class CScene;

protected:
    CSpriteComponent();
    CSpriteComponent(const CSpriteComponent& component);
    virtual ~CSpriteComponent();

protected:
    std::shared_ptr<CAnimation> m_Animation;

public:
    std::string GetAnimationName()  const
    {
        if (!m_Animation)
        {
            return "None";
        }

        return m_Animation->GetAnimationClassName();
    }

    CAnimation* GetAnimation() const
    {
        return m_Animation.get();
    }

    void GetAnimationNames(std::vector<std::string>& vecNames);

public:
    bool SetTexture(class CTexture* Texture);
    bool SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    bool SetTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
    void SetTextureFrameIndex(int Index);

    class CTexture* GetTexture(int Index = 0) const;

    void ClearAnimation();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CSpriteComponent* Clone() const;
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    template <typename T>
    T* SetAnimation(const std::string& Name)
    {
        std::shared_ptr<T> Anim;

        Anim->SetName(Name);
        Anim->m_Owner = this;

        if (!Anim->Init())
        {
            SAFE_DELETE(Anim);
            return nullptr;
        }

        m_Animation = Anim;

        return Anim;
    }
};

