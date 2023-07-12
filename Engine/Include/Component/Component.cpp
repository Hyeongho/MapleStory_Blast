#include "Component.h"

std::unordered_map<std::string, CComponent*> CComponent::m_mapComponent;

CComponent::CComponent() : m_Scene(nullptr), m_Owner(nullptr), m_SerialNumber(0)
{
	SetTypeID<CComponent>();

	m_ComponentTypeName = "Component";
}

CComponent::CComponent(const CComponent& Obj) : CRef(Obj)
{
	m_ComponentType = Obj.m_ComponentType;
	m_ComponentTypeName = Obj.m_ComponentTypeName;
	m_SerialNumber = Obj.m_SerialNumber;
}

CComponent::~CComponent()
{
}

void CComponent::Destroy()
{
	CRef::Destroy();
}

void CComponent::Start()
{
}

bool CComponent::Init()
{
	return true;
}

void CComponent::Update(float DeltaTime)
{
}

void CComponent::PostUpdate(float DeltaTime)
{
}

void CComponent::Render()
{
}

CComponent* CComponent::Clone() const
{
	return nullptr;
}

void CComponent::Save(FILE* File)
{
}

void CComponent::Load(FILE* File)
{
}
