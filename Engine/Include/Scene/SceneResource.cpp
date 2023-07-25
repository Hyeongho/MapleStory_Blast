#include "SceneResource.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
	{
		auto iter = m_mapMesh.begin();
		auto iterEnd = m_mapMesh.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapMesh.erase(iter);
			iterEnd = m_mapMesh.end();

			CResourceManager::GetInst()->ReleaseMesh(Name);
		}
	}

	{
		auto iter = m_mapShader.begin();
		auto iterEnd = m_mapShader.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapShader.erase(iter);
			iterEnd = m_mapShader.end();

			CResourceManager::GetInst()->ReleaseShader(Name);
		}
	}

	{
		auto iter = m_mapTexture.begin();
		auto iterEnd = m_mapTexture.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			iter = m_mapTexture.erase(iter);
			iterEnd = m_mapTexture.end();

			CResourceManager::GetInst()->ReleaseTexture(Name);
		}
	}
}

bool CSceneResource::Init()
{
	return true;
}

bool CSceneResource::CreateMesh(MeshType Type, const std::string& Name, void* VtxData, int Size, int Count, D3D11_USAGE VtxUsage, D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IdxData, int IdxSize, int IdxCount, D3D11_USAGE IdxUsage, DXGI_FORMAT Fmt)
{
	if (FindMesh(Name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->CreateMesh(m_Owner, Type, Name, VtxData, Size, Count,VtxUsage, Primitive, IdxData, IdxSize,
		IdxCount, IdxUsage, Fmt))
	{
		return false;
	}

	m_mapMesh.insert(std::make_pair(Name, std::make_shared<CMesh>(CResourceManager::GetInst()->FindMesh(Name))));

	return true;
}

std::shared_ptr<CMesh> CSceneResource::FindMesh(const std::string& Name)
{
	auto iter = m_mapMesh.find(Name);

	if (iter == m_mapMesh.end())
	{
		std::shared_ptr<CMesh> Mesh = CResourceManager::GetInst()->FindMesh(Name);

		if (!Mesh)
		{
			return nullptr;
		}

		m_mapMesh.insert(std::make_pair(Name,Mesh));

		return Mesh;
	}

	return iter->second;
}

std::shared_ptr<CShader> CSceneResource::FindShader(const std::string& Name)
{
	auto iter = m_mapShader.find(Name);

	if (iter == m_mapShader.end())
	{
		std::shared_ptr<CShader> Shader = CResourceManager::GetInst()->FindShader(Name);

		if (!Shader)
		{
			return nullptr;
		}

		m_mapShader.insert(std::make_pair(Name, Shader));

		return Shader;
	}

	return iter->second;
}

bool CSceneResource::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	if (FindTexture(Name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
	{
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, std::make_shared<CTexture>(CResourceManager::GetInst()->FindTexture(Name))));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	if (FindTexture(Name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
	{
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, std::make_shared<CTexture>(CResourceManager::GetInst()->FindTexture(Name))));

	return true;
}

bool CSceneResource::LoadTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	if (FindTexture(Name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
	{
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, std::make_shared<CTexture>(CResourceManager::GetInst()->FindTexture(Name))));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	if (FindTexture(Name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath))
	{
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, std::make_shared<CTexture>(CResourceManager::GetInst()->FindTexture(Name))));

	return true;
}

bool CSceneResource::LoadTextureArray(const std::string& Name, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	return false;
}

bool CSceneResource::LoadTextureArrayFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
	return false;
}

bool CSceneResource::CreateTarget(const std::string& Name, unsigned int Width, unsigned int Height, DXGI_FORMAT PixelFormat, DXGI_FORMAT DepthFormat)
{
	return false;
}

void CSceneResource::RenderTexture()
{
}

std::shared_ptr<CTexture> CSceneResource::FindTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
	{
		std::shared_ptr<CTexture> Texture = CResourceManager::GetInst()->FindTexture(Name);

		if (!Texture)
		{
			return nullptr;
		}

		m_mapTexture.insert(std::make_pair(Name, Texture));

		return Texture;
	}

	return iter->second;
}

std::shared_ptr<CMaterial> CSceneResource::FindMaterial(const std::string& Name)
{
	auto iter = m_mapMaterial.find(Name);

	if (iter == m_mapMaterial.end())
	{
		std::shared_ptr<CMaterial> Material = CResourceManager::GetInst()->FindMaterial(Name);

		if (!Material)
		{
			return nullptr;
		}

		m_mapMaterial.insert(std::make_pair(Name, Material));

		return Material;
	}

	return iter->second;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	if (FindAnimationSequence2D(Name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(Name, TextureName, FileName, PathName))
	{
		return false;
	}

	m_mapAnimationSequence2D.insert(std::make_pair(Name, std::make_shared<CAnimationSequence>(CResourceManager::GetInst()->FindAnimationSequence2D(Name))));

	return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& Name, CTexture* Texture)
{
	if (FindAnimationSequence2D(Name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(Name, Texture))
	{
		return false;
	}

	m_mapAnimationSequence2D.insert(std::make_pair(Name, std::make_shared<CAnimationSequence>(CResourceManager::GetInst()->FindAnimationSequence2D(Name))));

	return true;
}

bool CSceneResource::CreateAnimationSequence2DFullPath(const std::string& Name, const std::string& TextureName, const TCHAR* FullPath)
{
	if (FindAnimationSequence2D(Name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->CreateAnimationSequence2DFullPath(Name, TextureName, FullPath))
	{
		return false;
	}

	m_mapAnimationSequence2D.insert(std::make_pair(Name, std::make_shared<CAnimationSequence>(CResourceManager::GetInst()->FindAnimationSequence2D(Name))));

	return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName, const std::vector<const TCHAR*>& vecFileName, const std::string& PathName)
{
	if (FindAnimationSequence2D(Name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->CreateAnimationSequence2D(Name, TextureName, vecFileName, PathName))
	{
		return false;
	}

	m_mapAnimationSequence2D.insert(std::make_pair(Name, std::make_shared<CAnimationSequence>(CResourceManager::GetInst()->FindAnimationSequence2D(Name))));

	return true;
}

bool CSceneResource::CreateAnimationSequence2DFullPath(const std::string& Name, const std::string& TextureName, const std::vector<const TCHAR*>& vecFullPath)
{
	if (FindAnimationSequence2D(Name))
	{
		return true;
	}

	if (!CResourceManager::GetInst()->CreateAnimationSequence2DFullPath(Name, TextureName, vecFullPath))
	{
		return false;
	}

	m_mapAnimationSequence2D.insert(std::make_pair(Name, std::make_shared<CAnimationSequence>(CResourceManager::GetInst()->FindAnimationSequence2D(Name))));

	return true;
}

bool CSceneResource::AddAnimationSequence2DFrame(const std::string& Name, const Vector2& Start, const Vector2& End)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrame(Name, Start, End);
}

bool CSceneResource::AddAnimationSequence2DFrame(const std::string& Name, float StartX, float StartY, float EndX, float EndY)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrame(Name, StartX, StartY, EndX, EndY);
}

bool CSceneResource::AddAnimationSequence2DFrameAll(const std::string& Name, int Count, const Vector2& Start, const Vector2& End)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrameAll(Name, Count, Start, End);
}

bool CSceneResource::AddAnimationSequence2DFrameAll(const std::string& Name, int Count, float StartX, float StartY, float EndX, float EndY)
{
	return CResourceManager::GetInst()->AddAnimationSequence2DFrameAll(Name, Count, StartX, StartY, EndX, EndY);
}

bool CSceneResource::SaveSequence2D(const std::string& Name, const char* FullPath)
{
	return CResourceManager::GetInst()->SaveSequence2D(Name, FullPath);
}

bool CSceneResource::LoadSequence2D(const std::string& Name, const char* FullPath)
{
	return CResourceManager::GetInst()->LoadSequence2D(Name, FullPath);
}

bool CSceneResource::SaveSequence2D(const std::string& Name, const char* FileName, const std::string& PathName)
{
	return CResourceManager::GetInst()->SaveSequence2D(Name, FileName, PathName);
}

bool CSceneResource::LoadSequence2D(const std::string& Name, const char* FileName, const std::string& PathName)
{
	return CResourceManager::GetInst()->LoadSequence2D(Name, FileName, PathName);
}

CAnimationSequence* CSceneResource::FindAnimationSequence2D(const std::string& Name)
{
	auto iter = m_mapAnimationSequence2D.find(Name);

	if (iter == m_mapAnimationSequence2D.end())
	{
		CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence2D(Name);

		if (!Sequence)
		{
			return nullptr;
		}

		m_mapAnimationSequence2D.insert(std::make_pair(Name, std::make_shared<CAnimationSequence>(Sequence)));

		return Sequence;
	}

	return iter->second.get();
}

bool CSceneResource::CreateSoundChannel(const std::string& Name)
{
	return false;
}

bool CSceneResource::LoadSound(const std::string& GroupName, const std::string& Name, bool Loop, const char* FileName, const std::string& PathName)
{
	return false;
}

bool CSceneResource::SetVolume(int Volume)
{
	return false;
}

bool CSceneResource::SetVolume(const std::string& GroupName, int Volume)
{
	return false;
}

bool CSceneResource::SoundPlay(const std::string& Name)
{
	return false;
}

bool CSceneResource::SoundStop(const std::string& Name)
{
	return false;
}

bool CSceneResource::SoundPause(const std::string& Name)
{
	return false;
}

bool CSceneResource::SoundResume(const std::string& Name)
{
	return false;
}

FMOD::ChannelGroup* CSceneResource::FindChannelGroup(const std::string& Name)
{
	return nullptr;
}

CSound* CSceneResource::FindSound(const std::string& Name)
{
	return nullptr;
}

bool CSceneResource::CreateFontCollection(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	return false;
}

bool CSceneResource::LoadFont(const std::string& Name, const TCHAR* FontName, int Weight, float FontSize, const TCHAR* LocalName, int Stretch)
{
	return false;
}

const TCHAR* CSceneResource::GetFontFaceName(const std::string& CollectionName)
{
	return nullptr;
}

const char* CSceneResource::GetFontFaceNameMultibyte(const std::string& CollectionName)
{
	return nullptr;
}

bool CSceneResource::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return false;
}

bool CSceneResource::CreateFontColor(float r, float g, float b, float a)
{
	return false;
}

bool CSceneResource::CreateFontColor(const Vector4& Color)
{
	return false;
}

bool CSceneResource::CreateFontColor(unsigned int Color)
{
	return false;
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return nullptr;
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(float r, float g, float b, float a)
{
	return nullptr;
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(const Vector4& Color)
{
	return nullptr;
}

ID2D1SolidColorBrush* CSceneResource::FindFontColor(unsigned int Color)
{
	return nullptr;
}

unsigned int CSceneResource::CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return 0;
}

unsigned int CSceneResource::CreateFontColorKey(float r, float g, float b, float a)
{
	return 0;
}

unsigned int CSceneResource::CreateFontColorKey(const Vector4& Color)
{
	return 0;
}

std::shared_ptr<CFont> CSceneResource::FindFont(const std::string& Name)
{
	return std::shared_ptr<CFont>();
}

CFontCollection* CSceneResource::FindFontCollection(const std::string& Name)
{
	return nullptr;
}
