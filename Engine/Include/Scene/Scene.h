#pragma once

#include "SceneInfo.h"
#include "SceneResource.h"
#include "CameraManager.h"
#include "SceneCollision.h"
#include "SceneViewport.h"

class CScene
{
	friend class CSceneManager;

private:
	CScene();
	~CScene();

private:
	static std::unordered_map<std::string, CSceneInfo*>	m_mapSceneInfoCDO;

private:
	static void CreateCDO();

public:
	template <typename T>
	static T* CreateObjectCDO(const std::string& Name)
	{
		T* CDO = new T;

		CDO->Init();

		CGameObject::AddObjectCDO(Name, (CGameObject*)CDO);

		return CDO;
	}

	static void AddSceneInfoCDO(const std::string& Name, CSceneInfo* Info)
	{
		m_mapSceneInfoCDO.insert(std::make_pair(Name, Info));
	}

	static CSceneInfo* FindSceneInfoCDO(const std::string& Name)
	{
		auto iter = m_mapSceneInfoCDO.find(Name);

		if (iter == m_mapSceneInfoCDO.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	template <typename T>
	static T* CreateUIWindowCDO(const std::string& Name)
	{
		T* CDO = new T;

		CDO->Init();

		CUIWindow::AddUIWindowCDO(Name, (CUIWindow*)CDO);

		return CDO;
	}

	template <typename T>
	static T* CreateUIWidgetCDO(const std::string& Name)
	{
		T* CDO = new T;

		CDO->Init();

		CUIWidget::AddUIWidgetCDO(Name, (CUIWidget*)CDO);

		return CDO;
	}

private:
	bool m_Change;
	bool m_Start;
	CSceneInfo* m_SceneInfo;
	CSceneResource* m_Resource;
	CCameraManager* m_CameraManager;
	CSceneCollision* m_CollisionManager;
	CSceneViewport* m_Viewport;
	std::list<CSharedPtr<class CGameObject>> m_ObjList;
	std::string	m_Name;
	std::function<void(float)>	m_LoadingCallback;

public:
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	const std::string& GetName() const
	{
		return m_Name;
	}

	CSceneInfo* GetSceneInfo() const
	{
		return m_SceneInfo;
	}

	CSceneResource* GetResource() const
	{
		return m_Resource;
	}

	CCameraManager* GetCameraManager() const
	{
		return m_CameraManager;
	}

	CSceneCollision* GetCollisionManager() const
	{
		return m_CollisionManager;
	}

	CSceneViewport* GetViewport() const
	{
		return m_Viewport;
	}

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Collision(float DeltaTime);
	void Save(const char* FullPath);
	void Load(const char* FullPath);
	void Save(const char* FileName, const std::string& PathName);
	void Load(const char* FileName, const std::string& PathName);
	void GetAllGameObjectHierarchyName(std::vector<HierarchyObjectName>& vecName);

public:
	class CGameObject* FindObject(const std::string& Name);

public:
	template <typename T>
	bool CreateSceneInfo()
	{
		SAFE_DELETE(m_SceneInfo);

		m_SceneInfo = new T;

		m_SceneInfo->m_Owner = this;

		if (!m_SceneInfo->Init())
		{
			SAFE_DELETE(m_SceneInfo);
			return false;
		}

		return true;
	}

	template <typename T>
	T* CreateObject(const std::string& Name)
	{
		T* Obj = new T;

		Obj->SetName(Name);
		Obj->SetScene(this);

		if (!Obj->Init())
		{
			SAFE_RELEASE(Obj);
			return nullptr;
		}

		m_ObjList.push_back(CSharedPtr<T>(Obj));

		if (m_Start)
		{
			Obj->Start();
		}

		return Obj;
	}

	template <typename T>
	void SetLoadingCallback(T* Obj, void(T::* Func)(float))
	{
		m_LoadingCallback = std::bind(Func, Obj, std::placeholders::_1);
	}
};

