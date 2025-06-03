#include "XmlMapParser.h"
#include "../Scene/SceneManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/SpriteComponent.h"
#include "../Animation/Animation.h"
#include "../Resource/ResourceManager.h"
#include "../PathManager.h"

using namespace tinyxml2;

namespace 
{
    Vector2 GetVector2(XMLElement* parent, const char* nameX, const char* nameY)
    {
        int x = 0, y = 0;
        if (auto* xElem = parent->FirstChildElement(nameX))
        {
            xElem->QueryIntAttribute("value", &x);
        }
        if (auto* yElem = parent->FirstChildElement(nameY))
        {
            yElem->QueryIntAttribute("value", &y);
        }

        return Vector2((float)x, (float)y);
    }
}

bool CXmlMapParser::Parse(const std::string& filePath, CScene* scene)
{
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS)
    {
        return false;
    }

    auto* root = doc.FirstChildElement("dir");

    for (auto* node = root->FirstChildElement("dir"); node; node = node->NextSiblingElement("dir"))
    {
        const std::string name = node->Attribute("name");

        if (name == "info")
        {
            ParseInfo(node);
        }

        else if (name == "back")
        {
            ParseBack(node, scene);
        }

        else if (name == "foothold")
        {
            ParseFoothold(node, scene);
        }

        else if (name == "ladderRope")
        {
            ParseLadderRope(node, scene);
        }
    }

    return true;
}

void CXmlMapParser::ParseInfo(tinyxml2::XMLElement* element)
{
}

void CXmlMapParser::ParseBack(tinyxml2::XMLElement* element, CScene* scene)
{
    for (auto* layer = element->FirstChildElement("dir"); layer; layer = layer->NextSiblingElement("dir"))
    {
        const char* layerName = layer->Attribute("name");
        std::string spineName;
        Vector2 position;

        for (auto* child = layer->FirstChildElement(); child; child = child->NextSiblingElement())
        {
            const char* key = child->Attribute("name");
            if (!key)
            {
                continue;
            }

            if (strcmp(key, "spineAni") == 0)
            {
                spineName = child->Attribute("value");
            }

            else if (strcmp(key, "x") == 0)
            {
                int tempX = 0;
                child->QueryIntAttribute("value", &tempX);
                position.x = (float)tempX;
            }

            else if (strcmp(key, "y") == 0)
            {
                int tempY = 0;
                child->QueryIntAttribute("value", &tempY);
                position.y = (float)tempY;
            }
        }

        if (spineName.empty())
        {
            continue;
        }

        std::string texName = spineName;
        std::string texPath = texName + ".png";

        CTexture* texture = CResourceManager::GetInst()->FindTexture(texName);

        if (!texture)
        {
            CResourceManager::GetInst()->LoadTexture(texName, texPath.c_str(), TEXTURE_PATH);
            texture = CResourceManager::GetInst()->FindTexture(texName);
        }

        if (!texture) continue;

        if (!CResourceManager::GetInst()->FindAnimationSequence2D(texName))
        {
            CResourceManager::GetInst()->CreateAnimationSequence2D(texName, texture);
            CResourceManager::GetInst()->AddAnimationSequence2DFrame(texName, Vector2(0.f, 0.f), Vector2(1.f, 1.f));
        }

        Vector3 worldPos(position.x, position.y, 0.f);

        CGameObject* obj = scene->CreateObject<CGameObject>(texName);
        obj->SetWorldPosition(worldPos);

        CSpriteComponent* sprite = obj->CreateComponent<CSpriteComponent>("BGSprite");
        sprite->SetAnimation<CAnimation>(texName);
    }
}

void CXmlMapParser::ParseFoothold(tinyxml2::XMLElement* element, CScene* scene)
{
}

void CXmlMapParser::ParseLadderRope(tinyxml2::XMLElement* element, CScene* scene)
{
}
