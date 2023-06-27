#pragma once

#include "EngineInfo.h"

class CRef
{
public:
	CRef();
	CRef(const CRef& ref);
	virtual ~CRef();

protected:
	int m_RefCount;
	std::string m_Name;
	std::string m_TypeName;
	size_t m_TypeID;
	bool m_Enable;
	bool m_Active;

public:
	int GetRefCount() const
	{
		return m_RefCount;
	}

	bool GetEnable() const
	{
		return m_Enable;
	}

	bool GetActive() const
	{
		return m_Active;
	}

	size_t GetTypeID() const
	{
		return m_TypeID;
	}

	const std::string& GetTypeName() const
	{
		return m_TypeName;
	}

	const std::string& GetName() const
	{
		return m_Name;
	}

	void SetEnable(bool Enable)
	{
		m_Enable = Enable;
	}

	virtual void Destroy()
	{
		m_Active = false;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	template <typename T>
	bool CheckTypeID() const
	{
		return m_TypeID == typeid(T).hash_code();
	}

public:
	template <typename T>
	void SetTypeID()
	{
		m_TypeName = typeid(T).name();

		m_TypeID = typeid(T).hash_code();
	}
};

