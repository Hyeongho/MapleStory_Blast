#pragma once

#include "GameInfo.h"

class CWzArchive
{
public:
	CWzArchive(const std::string& path);
	~CWzArchive();

public:
	std::string m_fileName;
	//WzType m_type;
	bool m_loaded;
	//WzNode* rootNode;

public:
	bool Parse();
	void Close();

private:
	//std::ifstream m_fileStream;
	//WzHeader m_header;
};

