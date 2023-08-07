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

	Material = CreateMaterial<CMaterial>("TileMap");

	Material->SetShader("TileMapShader");

	Material->SetRenderState("DepthLessEqual");
	Material->SetRenderState("AlphaBlend");

	Material = CreateMaterial<CMaterial>("DefaultTileMapBack");

	Material->SetShader("TileMapBackShader");
	Material->SetRenderState("DepthLessEqual");

	return true;
}

CSharedPtr<CMaterial> CMaterialManager::FindMaterial(const std::string& Name)
{
	auto iter = m_mapMaterial.find(Name);

	if (iter == m_mapMaterial.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CMaterialManager::ReleaseMaterial(const std::string& Name)
{
	auto iter = m_mapMaterial.find(Name);

	if (iter != m_mapMaterial.end())
	{
		if (iter->second->GetRefCount() == 1)
		{
			m_mapMaterial.erase(iter);
		}
	}
}
