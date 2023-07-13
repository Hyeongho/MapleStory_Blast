#include "MaterialManager.h"

CMaterialManager::CMaterialManager()
{
}

CMaterialManager::~CMaterialManager()
{
}

bool CMaterialManager::Init()
{
	CMaterial* Material = CreateMaterial<CMaterial>("DefaultColor");

	Material->SetShader("SpriteColorShader");

	Material = CreateMaterial<CMaterial>("DefaultTexture");

	Material->SetShader("SpriteShader");

	Material->AddTexture(0, (int)EShaderBufferType::Pixel, "DefaultTexture", TEXT("teemo.png"));

	return true;
}

CMaterial* CMaterialManager::FindMaterial(const std::string& Name)
{
	auto iter = m_mapMaterial.find(Name);

	if (iter == m_mapMaterial.end())
	{
		return nullptr;
	}

	return iter->second.get();
}

void CMaterialManager::ReleaseMaterial(const std::string& Name)
{
	auto iter = m_mapMaterial.find(Name);

	if (iter != m_mapMaterial.end())
	{
		if (iter->second.use_count() == 1)
		{
			m_mapMaterial.erase(iter);
		}
	}
}
