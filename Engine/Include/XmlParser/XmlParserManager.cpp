#include "XmlParserManager.h"
#include "XmlParserBase.h"

void CXmlParserManager::Register(const std::string& type, CXmlParserBase* parser)
{
	m_ParserMap[type] = parser;
}

bool CXmlParserManager::ParseXml(const std::string& type, const std::string& filePath, CScene* scene)
{
    auto it = m_ParserMap.find(type);

    if (it == m_ParserMap.end())
    {
        return false;
    }

    return it->second->Parse(filePath, scene);
}
