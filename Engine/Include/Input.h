#pragma once

#include "EngineInfo.h"

#define	DIK_MOUSELBUTTON	0xfc
#define	DIK_MOUSERBUTTON	0xfd
#define	DIK_MOUSEWHEEL		0xfe

enum class InputSystem_Type
{
	DInput,
	Window
};

enum class Input_Type
{
	Down,
	Push,
	Up,
	End
};

struct KeyState
{
	unsigned char key;
	bool Down;	
	bool Push;
	bool Up;

	KeyState() : key(0), Down(false), Push(false), Up(false)
	{
	}
};

struct BindFunction
{
	void* Obj;
	std::function<void()> func;
	class CScene* Scene;

	BindFunction() : Obj(nullptr), Scene(nullptr)
	{
	}
};

struct BindKey
{
	std::string	Name;

	KeyState* key;
	bool Ctrl;
	bool Alt;
	bool Shift;

	std::vector<BindFunction*> vecFunction[(int)Input_Type::End];

	BindKey() : key(nullptr), Ctrl(false), Alt(false), Shift(false)
	{
	}
};

class CInput
{
private:
	std::unordered_map<unsigned char, KeyState*> m_mapKeyState;
	std::unordered_map<std::string, BindKey*> m_mapBindKey;
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;
	HWND m_hWnd;
	IDirectInput8* m_Input;
	IDirectInputDevice8* m_Keyboard;
	IDirectInputDevice8* m_Mouse;
	unsigned char m_KeyArray[256];
	DIMOUSESTATE m_MouseState;
	InputSystem_Type m_InputSystem;

	Vector2	m_MousePos;
	Vector2	m_MouseWorldPos;
	Vector2	m_MouseMove;
	bool m_MouseLDown;
	bool m_MouseLPush;
	bool m_MouseLUp;
	bool m_ShowCursor;
	bool m_CollisionWidget;

public:
	bool GetMouseLDown() const
	{
		return m_MouseLDown;
	}

	bool GetMouseLPush() const
	{
		return m_MouseLPush;
	}

	bool GetMouseLUp() const
	{
		return m_MouseLUp;
	}

	const Vector2& GetMousePos() const
	{
		return m_MousePos;
	}

	const Vector2& GetMouseWorldPos() const
	{
		return m_MouseWorldPos;
	}

	const Vector2& GetMouseMove() const
	{
		return m_MouseMove;
	}

public:
	void ComputeWorldMousePos(const Vector2& CameraPos);

private:
	bool InitDirectInput();
	void ReadDirectInputKeyboard();
	void ReadDirectInputMouse();

public:
	bool Init(HINSTANCE hInst, HWND hWnd);
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);

private:
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState(float DeltaTime);
	void UpdateBindKey(float DeltaTime);

	void SetKeyCtrl(const std::string& Name, bool Ctrl = true);
	void SetKeyAlt(const std::string& Name, bool Alt = true);
	void SetKeyShift(const std::string& Name, bool Shift = true);

	KeyState* FindKeyState(unsigned char Key);
	BindKey* FindBindKey(const std::string& Name);

public:
	bool AddBindKey(const std::string& Name, unsigned char Key);
	void ClearCallback();
	void ClearCallback(class CScene* Scene);

private:
	unsigned char ConvertKey(unsigned char Key);

public:
	template <typename T>
	void AddBindFunction(const std::string& KeyName, Input_Type Type,
		T* Object, void (T::* Func)(), class CScene* Scene)
	{
		BindKey* Key = FindBindKey(KeyName);

		if (!Key)
		{
			return;
		}

		BindFunction* Function = new BindFunction;

		Function->Obj = Object;
		Function->func = std::bind(Func, Object);
		Function->Scene = Scene;

		Key->vecFunction[(int)Type].push_back(Function);
	}

	template <typename T>
	bool DeleteBindFunction(const std::string& Name, Input_Type Type, T* Obj)
	{
		BindKey* Key = FindBindKey(Name);

		if (!Key)
		{
			return false;
		}

		auto iter = Key->vecFunction[(int)Type].begin();
		auto iterEnd = Key->vecFunction[(int)Type].end();

		for (; iter != iterEnd;)
		{
			if ((*iter)->Obj == Obj)
			{
				SAFE_DELETE((*iter));
				iter = Key->vecFunction[(int)Type].erase(iter);
				iterEnd = Key->vecFunction[(int)Type].end();
				continue;
			}

			iter++;
		}

		return true;
	}

	DECLARE_SINGLE(CInput)
};

