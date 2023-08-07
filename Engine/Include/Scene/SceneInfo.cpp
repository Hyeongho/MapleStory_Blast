#include "SceneInfo.h"
#include "../GameObject/GameObject.h"
#include "Scene.h"

CSceneInfo::CSceneInfo()
{
	m_ClassTypeName = "SceneInfo";
}

CSceneInfo::CSceneInfo(const CSceneInfo& Info)
{
	m_ClassTypeName = Info.m_ClassTypeName;
}

CSceneInfo::~CSceneInfo()
{
}

void CSceneInfo::SetPlayerObject(CGameObject* Player)
{
	m_PlayerObject = Player;

	if (Player)
	{
		m_PlayerObjectName = Player->GetName();
	}
}

void CSceneInfo::LoadComplete()
{
	if (!m_PlayerObjectName.empty())
	{
		m_PlayerObject = m_Owner->FindObject(m_PlayerObjectName);
	}
}

bool CSceneInfo::Init()
{
	return true;
}

void CSceneInfo::Update(float DeltaTime)
{
}

void CSceneInfo::PostUpdate(float DeltaTime)
{
}

CSceneInfo* CSceneInfo::Clone()
{
	return new CSceneInfo(*this);
}

void CSceneInfo::Save(FILE* File)
{
}

void CSceneInfo::Load(FILE* File)
{
}

void CSceneInfo::SceneChangeComplete()
{
}
