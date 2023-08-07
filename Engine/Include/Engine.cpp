#include "Engine.h"
#include "Device.h"
#include "Resource\ResourceManager.h"
#include "Timer.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "Input.h"
#include "Editor/EditorGUIManager.h"
#include "CollisionManager.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

float g_DeltaTime;

std::function<bool(HWND, UINT, WPARAM, LPARAM)> CEngine::m_WndProcFunc;

DEFINITION_SINGLE(CEngine)

bool CEngine::m_Loop = true;
bool CEngine::m_EditorMode = false;

CEngine::CEngine() : m_hInst(0), m_hWnd(0), m_WindowRS{}, m_ClearColor{}
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(61771);
}

CEngine::~CEngine()
{
	CSceneManager::DestroyInst();

	CEditorGUIManager::DestroyInst();

	CCollisionManager::DestroyInst();

	CRenderManager::DestroyInst();

	CInput::DestroyInst();

	CPathManager::DestroyInst();

	CResourceManager::DestroyInst();

	SAFE_DELETE(m_Timer);

	CDevice::DestroyInst();
}

float CEngine::GetFPS() const
{
	return m_Timer->GetFPS();
}

bool CEngine::Init(HINSTANCE hInst, const TCHAR* Title, const TCHAR* ClassName, int IconID, int SmallIconID, unsigned int WindowWidth, unsigned int WindowHeight, unsigned int DeviceWidth, unsigned int DeviceHeight, bool WindowMode, int MenuID)
{
	m_hInst = hInst;
	m_WindowRS.Width = WindowWidth;
	m_WindowRS.Height = WindowHeight;

	Register(ClassName, IconID, SmallIconID, MenuID);

	Create(Title, ClassName);

	// Device 초기화
	if (!CDevice::GetInst()->Init(m_hWnd, DeviceWidth, DeviceHeight, WindowMode))
	{
		return false;
	}

	// 경로관리자 초기화
	if (!CPathManager::GetInst()->Init())
	{
		return false;
	}

	// 렌더링 관리자 초기화
	if (!CRenderManager::GetInst()->Init())
	{
		return false;
	}

	// Resource 관리자 초기화
	if (!CResourceManager::GetInst()->Init())
	{
		return false;
	}

	// 충돌 관리자 초기화
	if (!CCollisionManager::GetInst()->Init())
	{
		return false;
	}


	// 입력 관리자 초기화
	if (!CInput::GetInst()->Init(m_hInst, m_hWnd))
	{
		return false;
	}

	if (m_EditorMode)
	{
		if (!CEditorGUIManager::GetInst()->Init(m_hWnd))
		{
			return false;
		}
	}

	// 장면관리자 초기화
	if (!CSceneManager::GetInst()->Init())
	{
		return false;
	}

	m_Timer = new CTimer;

	m_Timer->Init();

	return true;
}

int CEngine::Run()
{
	MSG msg;

	while (m_Loop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}

		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CEngine::Logic()
{
	m_Timer->Update();

	float DeltaTime = m_Timer->GetDeltaTime();

	g_DeltaTime = DeltaTime;

	Resolution RS = CDevice::GetInst()->GetResolution();

	CInput::GetInst()->Update(DeltaTime);

	if (m_EditorMode)
	{
		CEditorGUIManager::GetInst()->Update(DeltaTime);
	}

	CResourceManager::GetInst()->Update();

	Input(DeltaTime);

	if (Update(DeltaTime))
	{
		return;
	}

	if (PostUpdate(DeltaTime))
	{
		return;
	}

	Collision(DeltaTime);
	Render(DeltaTime);
}

void CEngine::Input(float DeltaTime)
{
}

bool CEngine::Update(float DeltaTime)
{
	bool Result = CSceneManager::GetInst()->Update(DeltaTime);

	return Result;
}

bool CEngine::PostUpdate(float DeltaTime)
{
	bool Result = CSceneManager::GetInst()->PostUpdate(DeltaTime);

	return Result;
}

bool CEngine::Collision(float DeltaTime)
{
	return CSceneManager::GetInst()->Collision(DeltaTime);
}

void CEngine::Render(float DeltaTime)
{
	CDevice::GetInst()->ClearRenderTarget(m_ClearColor);
	CDevice::GetInst()->ClearDepthStencil(1.f, 0);

	CDevice::GetInst()->RenderStart();

	// 그려진 백버퍼를 화면에 시연한다.
	CDevice::GetInst()->Flip();
}

void CEngine::Register(const TCHAR* ClassName, int IconID, int SmallIconID, int MenuID)
{
	// 레지스터에 등록할 윈도우 클래스 구조체를 만들어준다.
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// 메세지큐에서 꺼내온 메세지를 인자로 전달하며 호출할 함수의 함수 주소를
	// 등록한다.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	// 윈도우 인스턴스를 등록한다.
	wcex.hInstance = m_hInst;

	// 실행파일에 사용할 아이콘을 등록한다.
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));

	// 마우스 커서 모양을 결정한다.
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	// 메뉴를 사용할지 말지를 결정한다.
	if (MenuID != 0)
	{
		wcex.lpszMenuName = MAKEINTRESOURCEW(MenuID);
	}

	else
	{
		wcex.lpszMenuName = nullptr;
	}

	wcex.lpszClassName = ClassName;

	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(SmallIconID));

	RegisterClassExW(&wcex);
}

bool CEngine::Create(const TCHAR* Title, const TCHAR* ClassName)
{
	m_hWnd = CreateWindowW(ClassName, Title, WS_OVERLAPPEDWINDOW, 100, 0, 0, 0, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return false;
	}

	RECT rc = { 0, 0, (LONG)m_WindowRS.Width, (LONG)m_WindowRS.Height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	MoveWindow(m_hWnd, 50, 50, rc.right - rc.left, rc.bottom - rc.top, TRUE);

	ShowWindow(m_hWnd, SW_SHOW);

	UpdateWindow(m_hWnd);

	return true;
}

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_WndProcFunc)
	{
		if (m_WndProcFunc(hWnd, message, wParam, lParam))
		{
			return 1;
		}
	}

	switch (message)
	{
	case WM_DESTROY:
		// 윈도우가 종료될때 들어오는 메세지이다.
		m_Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
