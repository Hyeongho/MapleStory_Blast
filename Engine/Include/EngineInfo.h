#pragma once

#define DIRECTINP

#define	DIRECTINPUT_VERSION	0x0800

#include <Windows.h>
#include <vector>
#include <list>
#include <unordered_map>
#include <crtdbg.h>
#include <typeinfo>
#include <string>
#include <functional>
#include <algorithm>
#include <stack>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <filesystem>
#include <dwrite_3.h>
#include <d2d1.h>
#include <process.h>
#include <memory.h>
#include <thread>
#include <mutex>

#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "fmod.hpp"

#pragma comment(lib, "../Bin/fmod64_vc.lib")

extern float g_DeltaTime;

#define	ROOT_PATH "Root"
#define	SHADER_PATH "Shader"
#define	TEXTURE_PATH "Texture"
#define	SOUND_PATH "Sound"
#define	FONT_PATH "Font"
#define	ANIMATION2D_PATH "Animation2D"
#define	SCENE_PATH "Scene"

#define	DECLARE_SINGLE(Type) \
private:\
	static Type* m_Inst;\
public:\
	static Type* GetInst()\
	{\
		if (!m_Inst)\
		{\
			m_Inst = new Type;\
		}\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		if(m_Inst)\
		{\
			delete m_Inst;\
			m_Inst = nullptr;\
		}\
	}\
private:\
	Type();\
	~Type();

#define	DEFINITION_SINGLE(Type) Type* Type::m_Inst = nullptr;

#define	SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr;}

struct Resolution
{
	unsigned int Width;
	unsigned int Height;
};