#pragma once

#include "../EngineInfo.h"

class CCameraManager
{
	friend class CScene;

private:
	CCameraManager();
	~CCameraManager();

private:
	class CScene* m_Owner;

private:
	std::shared_ptr<class CCameraComponent>	m_CurrentCamera;
	std::shared_ptr<class CCameraComponent>	m_UICamera;

public:
	class CCameraComponent* GetCurrentCamera() const
	{
		return m_CurrentCamera.get();
	}

	class CCameraComponent* GetUICamera() const
	{
		return m_UICamera.get();
	}

public:
	void SetCurrentCamera(class CCameraComponent* Camera);
	void SetUICamera(class CCameraComponent* Camera);

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Save(FILE* File);
	void Load(FILE* File);
};

