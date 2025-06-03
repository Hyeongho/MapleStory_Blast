#pragma once

#include "../EngineInfo.h"

class CScene;
class CXmlParserBase;

class CXmlParserManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<CXmlParserBase>> m_ParserMap;

public:
    void Register(const std::string& type, std::shared_ptr<CXmlParserBase> parser);
    bool ParseXml(const std::string& type, const std::string& filePath, CScene* scene);
};

