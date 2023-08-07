#include "Animation.h"
#include "../Component/SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Shader/AnimationConstantBuffer.h"

std::unordered_map<std::string, CAnimation*> CAnimation::m_mapAnimationCDO;

CAnimation::CAnimation() : m_Owner(nullptr), m_Play(true)
{
	m_ClassName = "Animation";
}

CAnimation::CAnimation(const CAnimation& Anim) : CRef(Anim)
{
	m_Play = Anim.m_Play;

	m_mapAnimation.clear();

	auto iter = Anim.m_mapAnimation.begin();
	auto iterEnd = Anim.m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		CAnimationData* Data = iter->second->Clone();

		if (Data->GetName() == Anim.m_CurAnimation->GetName())
		{
			m_CurAnimation = Data;
		}

		m_mapAnimation.insert(std::make_pair(iter->first, Data));
	}
}

CAnimation::~CAnimation()
{
	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

CScene* CAnimation::GetScene() const
{
	if (!m_Owner)
	{
		return nullptr;
	}

	return m_Owner->GetScene();
}

void CAnimation::Start()
{
	if (m_Owner && m_CurAnimation)
	{
		m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
	}
}

bool CAnimation::Init()
{
	return true;
}

void CAnimation::Update(float DeltaTime)
{
	if (!m_Play || !m_CurAnimation || m_CurAnimation->m_Sequence->GetFrameCount() == 0)
	{
		return;
	}

	m_CurAnimation->m_Time += DeltaTime * m_CurAnimation->m_PlayScale;

	bool AnimEnd = false;

	m_CurAnimation->m_FrameTime = m_CurAnimation->m_PlayTime / m_CurAnimation->m_Sequence->GetFrameCount();

	if (m_CurAnimation->m_Time >= m_CurAnimation->m_FrameTime)
	{
		m_CurAnimation->m_Time -= m_CurAnimation->m_FrameTime;

		if (m_CurAnimation->m_Reverse)
		{
			m_CurAnimation->m_Frame--;

			if (m_CurAnimation->m_Frame < 0)
			{
				AnimEnd = true;
			}
		}

		else
		{
			m_CurAnimation->m_Frame++;

			if (m_CurAnimation->m_Frame == m_CurAnimation->m_Sequence->GetFrameCount())
			{
				AnimEnd = true;
			}
		}
	}

	size_t Size = m_CurAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!m_CurAnimation->m_vecNotify[i]->Call && m_CurAnimation->m_vecNotify[i]->Frame == m_CurAnimation->m_Frame)
		{
			m_CurAnimation->m_vecNotify[i]->Call = true;
			m_CurAnimation->m_vecNotify[i]->Function();
		}
	}

	if (AnimEnd)
	{
		if (m_CurAnimation->m_Loop)
		{
			if (m_CurAnimation->m_Reverse)
			{
				m_CurAnimation->m_Frame = m_CurAnimation->m_Sequence->GetFrameCount() - 1;
			}

			else
			{
				m_CurAnimation->m_Frame = 0;
			}

			Size = m_CurAnimation->m_vecNotify.size();

			for (size_t i = 0; i < Size; i++)
			{
				m_CurAnimation->m_vecNotify[i]->Call = false;
			}
		}

		else
		{
			if (m_CurAnimation->m_Reverse)
			{
				m_CurAnimation->m_Frame = 0;
			}

			else
			{
				m_CurAnimation->m_Frame = m_CurAnimation->m_Sequence->GetFrameCount() - 1;
			}
		}

		if (m_CurAnimation->m_EndFunction)
		{
			m_CurAnimation->m_EndFunction();
		}
	}
}

bool CAnimation::AddAnimation(const std::string& Name, const std::string& SequenceName, float PlayTime, float PlayScale, bool Loop, bool Reverse)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (Anim)
	{
		return false;
	}

	CAnimationSequence* Sequence = nullptr;
	//CSharedPtr<CAnimationSequence> Sequence = std::make_shared<CAnimationSequence>(new CAnimationSequence());

	if (m_Owner->GetScene())
	{
		Sequence = m_Owner->GetScene()->GetResource()->FindAnimationSequence2D(SequenceName);
	}

	else
	{
		Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(SequenceName);
	}

	Anim = new CAnimationData;

	Anim->m_Sequence = Sequence;
	Anim->m_SequenceName = SequenceName;
	Anim->m_Name = Name;
	Anim->m_PlayTime = PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_Loop = Loop;
	Anim->m_Reverse = Reverse;
	Anim->m_FrameTime = PlayTime / Sequence->GetFrameCount();
	Anim->m_Owner = this;

	if (m_mapAnimation.empty())
	{
		m_CurAnimation = Anim;

		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));

	return true;
}

bool CAnimation::AddAnimation(const std::string& Name, CAnimationSequence* Sequence, float PlayTime, float PlayScale, bool Loop, bool Reverse)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (Anim)
	{
		return false;
	}

	Anim = new CAnimationData;

	Anim->m_Sequence = Sequence;
	Anim->m_SequenceName = Sequence->GetName();
	Anim->m_Name = Name;
	Anim->m_PlayTime = PlayTime;
	Anim->m_PlayScale = PlayScale;
	Anim->m_Loop = Loop;
	Anim->m_Reverse = Reverse;
	Anim->m_FrameTime = PlayTime / Sequence->GetFrameCount();
	Anim->m_Owner = this;

	if (m_mapAnimation.empty())
	{
		m_CurAnimation = Anim;

		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		}
	}

	m_mapAnimation.insert(std::make_pair(Name, Anim));

	return true;
}

void CAnimation::SetPlayTime(const std::string& Name, float PlayTime)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (!Anim)
	{
		return;
	}

	Anim->m_PlayTime = PlayTime;
}

void CAnimation::SetPlayScale(const std::string& Name, float PlayScale)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (!Anim)
	{
		return;
	}

	Anim->m_PlayScale = PlayScale;
}

void CAnimation::SetLoop(const std::string& Name, bool Loop)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (!Anim)
	{
		return;
	}

	Anim->m_Loop = Loop;
}

void CAnimation::SetReverse(const std::string& Name, bool Reverse)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (!Anim)
	{
		return;
	}

	Anim->m_Reverse = Reverse;
}

void CAnimation::SetCurrentAnimation(const std::string& Name)
{
	CAnimationData* Anim = FindAnimation(Name);

	if (!Anim)
	{
		return;
	}

	m_CurAnimation = Anim;

	m_CurAnimation->m_Frame = 0;
	m_CurAnimation->m_Time = 0.f;

	size_t	Size = m_CurAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurAnimation->m_vecNotify[i]->Call = false;
	}

	if (m_Owner)
	{
		m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		m_Owner->SetTextureFrameIndex(0);
	}
}

void CAnimation::ChangeAnimation(const std::string& Name)
{
	if (m_CurAnimation->GetName() == Name)
	{
		return;
	}

	m_CurAnimation->m_Frame = 0;
	m_CurAnimation->m_Time = 0.f;

	size_t Size = m_CurAnimation->m_vecNotify.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_CurAnimation->m_vecNotify[i]->Call = false;
	}

	m_CurAnimation = FindAnimation(Name);

	m_CurAnimation->m_Frame = 0;
	m_CurAnimation->m_Time = 0.f;

	if (m_Owner)
	{
		m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
		m_Owner->SetTextureFrameIndex(0);
	}
}

void CAnimation::Save(FILE* File)
{
	CRef::Save(File);

	int	Count = (int)m_mapAnimation.size();

	fwrite(&Count, 4, 1, File);

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		iter->second->Save(File);
	}

	int	Length = (int)m_CurAnimation->GetName().length();

	fwrite(&Length, 4, 1, File);
	fwrite(m_CurAnimation->GetName().c_str(), 1, Length, File);
}

void CAnimation::Load(FILE* File)
{
	CRef::Load(File);

	auto iter = m_mapAnimation.begin();
	auto iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}

	m_mapAnimation.clear();

	m_CurAnimation = nullptr;


	int	Count = 0;

	fread(&Count, 4, 1, File);

	for (int i = 0; i < Count; i++)
	{
		CAnimationData* Data = new CAnimationData;

		Data->m_Owner = this;
		Data->Load(File);

		m_mapAnimation.insert(std::make_pair(Data->GetName(), Data));
	}

	int	Length = 0;
	char CurName[256] = {};

	fread(&Length, 4, 1, File);
	fread(CurName, 1, Length, File);

	m_CurAnimation = FindAnimation(CurName);
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::SetShader()
{
	if (!m_CurAnimation || !m_CurAnimation->m_Sequence || !m_CurAnimation->m_Sequence->GetTexture())
	{
		return;
	}

	CAnimationConstantBuffer* Buffer = CResourceManager::GetInst()->GetAnim2DConstantBuffer();

	const Animation2DFrameData& FrameData = m_CurAnimation->m_Sequence->GetFrameData(m_CurAnimation->m_Frame);

	EAnimation2DType Type = m_CurAnimation->m_Sequence->GetAnim2DType();

	if (Type == EAnimation2DType::Atlas)
	{
		Buffer->SetImageSize((float)m_CurAnimation->m_Sequence->GetTexture()->GetWidth(), (float)m_CurAnimation->m_Sequence->GetTexture()->GetHeight());
		Buffer->SetImageFrame(FrameData.Start, FrameData.End);
	}

	else if (Type == EAnimation2DType::Frame)
	{
		if (m_Owner)
		{
			m_Owner->SetTexture(m_CurAnimation->m_Sequence->GetTexture());
			m_Owner->SetTextureFrameIndex(m_CurAnimation->m_Frame);
		}

		else
		{
			m_CurAnimation->m_Sequence->GetTexture()->SetShader(0, (int)EShaderBufferType::Pixel, m_CurAnimation->m_Frame);
		}
	}

	else
	{
		Buffer->SetFrame(m_CurAnimation->m_Frame);
	}

	Buffer->SetImageType(m_CurAnimation->m_Sequence->GetAnim2DType());
	Buffer->SetAnim2DEnable(true);

	Buffer->UpdateBuffer();
}

CAnimationData* CAnimation::FindAnimation(const std::string& Name)
{
	auto iter = m_mapAnimation.find(Name);

	if (iter == m_mapAnimation.end())
	{
		return nullptr;
	}

	return iter->second;
}
