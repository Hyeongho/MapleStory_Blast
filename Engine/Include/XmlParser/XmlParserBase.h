#pragma once

#include "../EngineInfo.h"

class CXmlParserBase
{
public:
    virtual ~CXmlParserBase()
    {

    }

    virtual bool Parse(const std::string& filePath, class CScene* scene) = 0;
};

