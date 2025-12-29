#include "WzArchive.h"

#include <filesystem>
#include <iostream>

CWzArchive::CWzArchive(const std::string& filePath) : m_filePath(filePath), m_fileSize(0), m_headerSize(0), m_version(0), m_isParsed(false), m_type(WzType::Unknown)
{
    std::string stem = std::filesystem::path(filePath).stem().string();

    std::string lowerStem = stem;

    std::transform(lowerStem.begin(), lowerStem.end(), lowerStem.begin(), ::tolower);

    if (lowerStem == "base")      
    {
        m_type = WzType::Base;
    }

    else if (lowerStem == "character") 
    {
        m_type = WzType::Character;
    }

    else if (lowerStem == "effect")    
    {
        m_type = WzType::Effect;
    }

    else if (lowerStem == "item")      
    {
        m_type = WzType::Item;
    }

    else if (lowerStem == "map")       
    {
        m_type = WzType::Map;
    }

    else if (lowerStem == "quest")     
    {
        m_type = WzType::Quest;
    }

    else if (lowerStem == "skill")     
    {
        m_type = WzType::Skill;
    }

    else if (lowerStem == "sound")     
    {
        m_type = WzType::Sound;
    }
    else if (lowerStem == "string")    
    {
        m_type = WzType::String;
    }

    else if (lowerStem == "ui")        
    {
        m_type = WzType::UI;
    }

    else
    {
        m_type = WzType::Unknown;
    }
}

CWzArchive::~CWzArchive()
{
    Close();
}

bool CWzArchive::Parse()
{
    Close();

    m_fileStream.open(m_filePath, std::ios::binary);
    if (!m_fileStream.is_open()) {
        std::cerr << "Cannot open file: " << m_filePath << "\n";
        return false;
    }

    // 1) PKG1 시그니처 검사 (4바이트)
    char ident[5] = { 0 };
    m_fileStream.read(ident, 4);

    if (std::string(ident, 4) != "PKG1") 
    {
        std::cerr << "Invalid WZ signature: " << m_filePath << "\n";
        return false;
    }

    // 2) 파일 크기, 헤더 크기 읽기
    m_fileStream.read(reinterpret_cast<char*>(&m_fileSize), sizeof(m_fileSize));
    m_fileStream.read(reinterpret_cast<char*>(&m_headerSize), sizeof(m_headerSize));

    // 헤더 끝까지 건너뛰기 (이미 16바이트 읽었으므로 headerSize - 16 이동)
    if (m_headerSize > 16) 
    {
        m_fileStream.seekg(m_headerSize - 16, std::ios::cur);
    }

    // 3) 버전 읽기 (2바이트; 암호화된 값)
    m_fileStream.read(reinterpret_cast<char*>(&m_version), sizeof(m_version));
    // (여기서는 실제 복호화는 생략)

    // 4) 남은 데이터 읽기 (fileSize에는 헤더 제외 크기가 포함됨)
    // version(2바이트)을 제외한 나머지 크기 계산
    uint64_t dataSize = (m_fileSize > 2) ? (m_fileSize - 2) : 0;
    m_data.resize(static_cast<size_t>(dataSize));
    m_fileStream.read(m_data.data(), dataSize);

    // 5) zlib을 사용하여 압축 해제
    std::vector<char> out;
    const size_t CHUNK = 16384;
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    if (inflateInit(&strm) != Z_OK) 
    {
        std::cerr << "inflateInit failed\n";
        return false;
    }

    strm.next_in = reinterpret_cast<Bytef*>(m_data.data());
    strm.avail_in = static_cast<uInt>(m_data.size());

    do {
        size_t prevSize = out.size();
        out.resize(prevSize + CHUNK);
        strm.next_out = reinterpret_cast<Bytef*>(&out[prevSize]);
        strm.avail_out = CHUNK;

        int ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) 
        {
            std::cerr << "inflate stream error\n";
            inflateEnd(&strm);
            return false;
        }

        if (ret == Z_MEM_ERROR) 
        {
            std::cerr << "inflate memory error\n";
            inflateEnd(&strm);
            return false;
        }

        if (ret == Z_DATA_ERROR) 
        {
            std::cerr << "inflate data error\n";
            inflateEnd(&strm);
            return false;
        }

        // 완료 시 Z_STREAM_END를 리턴:contentReference[oaicite:7]{index=7} 
        if (ret == Z_STREAM_END) 
        {
            break;
        }

    } while (strm.avail_out == 0);

    inflateEnd(&strm);

    // 해제된 데이터를 data에 저장(또는 원하는 파싱을 수행)
    m_data.swap(out);

    // 6) 간단한 노드 구조 파싱 (예: 루트 노드만 확인)
    //    여기서는 구체적 파싱을 생략하고 성공 처리
    m_isParsed = true;
    return true;
}

void CWzArchive::Close()
{
    if (m_fileStream.is_open())
    {
        m_fileStream.close();
    }

    m_data.clear();
    m_isParsed = false;
}

std::string CWzArchive::getTypeName() const
{
    switch (m_type) {
    case WzType::Base:      
        return "Base";

    case WzType::Character: 
        return "Character";

    case WzType::Effect:    
        return "Effect";

    case WzType::Item:      
        return "Item";

    case WzType::Map:       
        return "Map";

    case WzType::Quest:     
        return "Quest";

    case WzType::Skill:     
        return "Skill";

    case WzType::Sound:     
        return "Sound";

    case WzType::String:    
        return "String";

    case WzType::UI:        
        return "UI";

    default:                
        return "Unknown";
    }
}