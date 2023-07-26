#include "SceneViewport.h"
#include "../GameObject/GameObject.h"
#include "Scene.h"
#include "../Input.h"

CSceneViewport::CSceneViewport()
{
}

CSceneViewport::~CSceneViewport()
{
}

void CSceneViewport::LoadComplete()
{
}

void CSceneViewport::Start()
{
	size_t Size = m_vecWindow.size();

	for (size_t i = 0; i < Size; i++)
	{
		m_vecWindow[i]->Start();
	}

	// ZOrder ������ ���� �����Ѵ�.
}

bool CSceneViewport::Init()
{
	return true;
}

void CSceneViewport::Update(float DeltaTime)
{
	auto iter = m_vecWindow.begin();
	auto iterEnd = m_vecWindow.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter).get()->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
			continue;
		}

		else if (!(*iter).get()->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter).get()->Update(DeltaTime);
		iter++;
	}
}

void CSceneViewport::PostUpdate(float DeltaTime)
{
	auto iter = m_vecWindow.begin();
	auto iterEnd = m_vecWindow.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter).get()->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
			continue;
		}

		else if (!(*iter).get()->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter).get()->PostUpdate(DeltaTime);
		iter++;
	}
}

void CSceneViewport::Render()
{
	if (m_vecWindow.size() >= 2)
	{
		std::sort(m_vecWindow.begin(), m_vecWindow.end(), CSceneViewport::SortWindow);
	}

	auto iter = m_vecWindow.begin();
	auto iterEnd = m_vecWindow.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter).get()->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
			continue;
		}

		else if (!(*iter).get()->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter).get()->Render();
		iter++;
	}
}

void CSceneViewport::Save(FILE* File)
{
	int	Count = (int)m_vecWindow.size();

	fwrite(&Count, sizeof(int), 1, File);

	auto iter = m_vecWindow.begin();
	auto iterEnd = m_vecWindow.end();

	for (; iter != iterEnd; iter++)
	{
		std::string	TypeName = (*iter).get()->GetWindowTypeName();

		int	Length = (int)TypeName.length();
		fwrite(&Length, sizeof(int), 1, File);
		fwrite(TypeName.c_str(), 1, Length, File);

		(*iter).get()->Save(File);
	}
}

void CSceneViewport::Load(FILE* File)
{
	int	Count = 0;

	fread(&Count, sizeof(int), 1, File);

	for (int i = 0; i < Count; i++)
	{
		char TypeName[256] = {};
		int	Length = 0;

		fread(&Length, sizeof(int), 1, File);
		fread(TypeName, 1, Length, File);

		CUIWindow* CDO = CUIWindow::FindCDO(TypeName);

		CUIWindow* Window = CDO->Clone();

		Window->m_Owner = this;
		Window->m_Scene = m_Owner;

		Window->Load(File);

		m_vecWindow.push_back(std::make_shared<CUIWindow>(Window));
	}
}

bool CSceneViewport::CollisionMouse()
{
	if (m_vecWindow.size() >= 2)
	{
		std::sort(m_vecWindow.begin(), m_vecWindow.end(), CSceneViewport::SortWindowInv);
	}

	if (m_CollisionWidget && !m_CollisionWidget->GetActive())
	{
		m_CollisionWidget = nullptr;
	}

	// ���콺 ��ġ�� ���´�.
	Vector2	MousePos = CInput::GetInst()->GetMousePos();

	auto iter = m_vecWindow.begin();
	auto iterEnd = m_vecWindow.end();

	bool Result = false;

	for (; iter != iterEnd; iter++)
	{
		if (!(*iter).get()->GetEnable())
		{
			continue;
		}

		CUIWidget* Widget = (*iter).get()->CollisionMouse(MousePos);

		if (m_CollisionWidget && m_CollisionWidget.get() != Widget)
		{
			m_CollisionWidget->m_MouseHovered = false;
		}

		if (Widget)
		{
			m_CollisionWidget = std::make_shared<CUIWidget>(Widget);
		}

		if (Widget)
		{
			Result = true;
			break;
		}
	}

	// �浹�� �ȵǾ��ٸ�
	if (!Result)
	{
		if (m_CollisionWidget)
		{
			m_CollisionWidget->m_MouseHovered = false;
		}

		m_CollisionWidget = nullptr;
	}

	iter = m_vecWindow.begin();
	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter).get()->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
			continue;
		}

		iter++;
	}

	return Result;
}

bool CSceneViewport::SortWindow(std::shared_ptr<CUIWindow> Src, std::shared_ptr<CUIWindow> Dest)
{
	return Src->GetZOrder() > Dest->GetZOrder();
}

bool CSceneViewport::SortWindowInv(std::shared_ptr<CUIWindow> Src, std::shared_ptr<CUIWindow> Dest)
{
	return Src->GetZOrder() < Dest->GetZOrder();
}
