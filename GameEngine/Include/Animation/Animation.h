#pragma once

#include "../Ref.h"
#include "AnimationData.h"

class CAnimation :
    public CRef
{
	friend class CSpriteComponent;
	friend class CSceneManager;
	friend class CScene;

protected:
	CAnimation();
	CAnimation(const CAnimation& Anim);
	virtual ~CAnimation();

private:
	static std::unordered_map<std::string, CAnimation*> m_mapAnimationCDO;

public:
	static void AddAnimationCDO(const std::string& Name, CAnimation* CDO)
	{
		m_mapAnimationCDO.insert(std::make_pair(Name, CDO));
	}

	static CAnimation* FindCDO(const std::string& Name)
	{
		auto iter = m_mapAnimationCDO.find(Name);

		if (iter == m_mapAnimationCDO.end())
		{
			return nullptr;
		}

		return iter->second;
	}

protected:
	std::string	m_ClassName;
	class CSpriteComponent* m_Owner;
	std::unordered_map<std::string, CAnimationData*> m_mapAnimation;
	CAnimationData* m_CurAnimation;
	bool m_Play;

public:
	class CScene* GetScene() const;

	const std::string& GetAnimationClassName() const
	{
		return m_ClassName;
	}

	void GetAnimationNames(std::vector<std::string>& vecNames)
	{
		auto iter = m_mapAnimation.begin();
		auto iterEnd = m_mapAnimation.end();

		for (; iter != iterEnd; iter++)
		{
			vecNames.push_back(iter->first);
		}
	}

public:
	void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	bool AddAnimation(const std::string& Name, const std::string& SequenceName, float PlayTime = 1.f, float PlayScale = 1.f, bool Loop = false, bool Reverse = false);
	bool AddAnimation(const std::string& Name, const CSharedPtr<CAnimationSequence>& Sequence, float PlayTime = 1.f, float PlayScale = 1.f, bool Loop = false, bool Reverse = false);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetLoop(const std::string& Name, bool Loop);
	void SetReverse(const std::string& Name, bool Reverse);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	virtual CAnimation* Clone();
	void SetShader();

public:
	CAnimationData* FindAnimation(const std::string& Name);

public:
	const std::string& GetCurrentAnimationName(const std::string& AnimationName) const
	{
		if (!m_CurAnimation)
		{
			return "";
		}

		return m_CurAnimation->GetName();
	}

	int GetCurrentFrame(const std::string& AnimationName) const
	{
		if (!m_CurAnimation)
		{
			return -1;
		}

		return m_CurAnimation->GetCurrentFrame();
	}

	float GetCurrentAnimationTime(const std::string& AnimationName)	const
	{
		if (!m_CurAnimation)
		{
			return -1.f;
		}

		return m_CurAnimation->GetAnimationTime();
	}

	class CAnimationSequence* GetCurrentAnimationSequence(const std::string& AnimationName)	const
	{
		if (!m_CurAnimation)
		{
			return nullptr;
		}

		return m_CurAnimation->GetAnimationSequence();
	}

public:
	template <typename T>
	void SetCurrentEndFunction(const std::string& AnimationName, T* Obj, void(T::* Func)())
	{
		CAnimationData* Animation = FindAnimation(AnimationName);

		if (!Animation)
		{
			return;
		}

		Animation->SetEndFunction<T>(Obj, Func);
	}

	template <typename T>
	void AddCurrentNotify(const std::string& AnimationName, const std::string& Name, int Frame, T* Obj, void(T::* Func)())
	{
		CAnimationData* Animation = FindAnimation(AnimationName);

		if (!Animation)
		{
			return;
		}

		Animation->AddNotify<T>(Name, Frame, Obj, Func);
	}

	template <typename T>
	void AddCurrentNotify(const std::string& AnimationName, const std::string& Name, float Time, T* Obj, void(T::* Func)())
	{
		CAnimationData* Animation = FindAnimation(AnimationName);

		if (!Animation)
		{
			return;
		}

		Animation->AddNotify<T>(Name, Time, Obj, Func);
	}
};

