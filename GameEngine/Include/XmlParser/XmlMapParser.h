#pragma once

#include "XmlParserBase.h"

class CXmlMapParser : public CXmlParserBase
{
public:
    virtual bool Parse(const std::string& filePath, class CScene* scene) override;

private:
    void ParseInfo(tinyxml2::XMLElement* element);
    void ParseBack(tinyxml2::XMLElement* element, class CScene* scene);
    void ParseFoothold(tinyxml2::XMLElement* element, class CScene* scene);
    void ParseLadderRope(tinyxml2::XMLElement* element, class CScene* scene);
};

