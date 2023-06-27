#include "Ref.h"

CRef::CRef() : m_RefCount(0), m_Enable(true), m_Active(true), m_TypeID(0)
{
}

CRef::CRef(const CRef& ref) : m_RefCount(0), m_Name(ref.m_Name), m_TypeName(ref.m_TypeName), m_TypeID(ref.m_TypeID), m_Enable(ref.m_Enable), m_Active(ref.m_Active)
{
}

CRef::~CRef()
{
}
