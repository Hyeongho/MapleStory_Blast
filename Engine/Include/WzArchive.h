#pragma once

#include "GameInfo.h"

#include <fstream>
#include <zlib.h>

enum class WzType
{
	Base, 
	Character,
	Effect,
	Item,
	Map,
	Quest,
	Skill,
	Sound,
	String,
	UI,
	Unknown,
};


class CWzArchive
{
	friend class CWzLoader;

public:
	CWzArchive(const std::string& filePath);
	~CWzArchive();

public:
	bool Parse();
	void Close();

	std::string getTypeName() const;

private:
	std::string m_filePath;
	std::ifstream m_fileStream;
	uint64_t m_fileSize;
	uint32_t m_headerSize;
	uint16_t m_version;
	bool m_isParsed;
	WzType m_type;

	std::vector<char> m_data;
};

