#include "WzLoader.h"

#include <filesystem>
#include <iostream>

bool CWzLoader::enable(const std::string& dir)
{
    namespace fs = std::filesystem;

    disable();

    try {
        for (const auto& entry : fs::directory_iterator(dir)) 
        {
            if (entry.is_regular_file() && entry.path().extension() == ".wz") 
            {
                auto wz = std::make_unique<CWzArchive>(entry.path().string());
                if (wz->Parse())
                {
                    archives.push_back(std::move(wz));
                }

                else 
                {
                    std::cerr << "Failed to parse WZ: " << entry.path() << "\n";
                }
            }
        }
    }

    catch (const fs::filesystem_error& e) 
    {
        std::cerr << "Filesystem error: " << e.what() << "\n";
        return false;
    }

    return !archives.empty();
}

void CWzLoader::disable()
{
    for (auto& wz : archives) 
    {
        wz->Close();
    }

    archives.clear();
}

void CWzLoader::printLoadedFiles() const
{
    for (const auto& wz : archives) 
    {
        std::filesystem::path p(wz->m_filePath);
        std::cout << p.filename().string() << " : " << wz->getTypeName() << "\n";
    }
}
