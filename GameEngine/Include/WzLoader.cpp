#include "WzLoader.h"

#include <filesystem>
#include <iostream>

static bool DebugOpenFile(const std::filesystem::path& p)
{
    std::wcout << L"[WZ] Try open: " << p.wstring() << L"\n";

    // 1) 존재 확인
    std::error_code ec;
    bool exists = std::filesystem::exists(p, ec);
    std::wcout << L"[WZ] exists=" << (exists ? L"true" : L"false")
        << L", ec=" << ec.value() << L"\n";
    if (!exists) return false;

    // 2) ifstream으로 열기
    std::ifstream f(p, std::ios::binary);
    std::cout << "[WZ] ifstream is_open=" << (f.is_open() ? "true" : "false") << "\n";
    if (f.is_open()) return true;

    // 3) Windows API로 에러 코드 확인 (원인 확정)
    HANDLE h = ::CreateFileW(
        p.c_str(), GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (h == INVALID_HANDLE_VALUE)
    {
        DWORD err = ::GetLastError();
        std::wcout << L"[WZ] CreateFileW failed. GetLastError=" << err << L"\n";
        return false;
    }

    ::CloseHandle(h);
    std::wcout << L"[WZ] CreateFileW ok (but ifstream failed) - path/CRT issue.\n";
    return false;
}

bool CWzLoader::enable(const std::string& dir)
{
    {
        // Raw string 권장
        std::filesystem::path baseWz = std::filesystem::path(R"(C:\Nexon\Maple\Data\Base\Base.wz)");
        bool ok = DebugOpenFile(baseWz);
        std::cout << "[WZ] Base.wz " << (ok ? "OPEN OK" : "OPEN FAIL") << "\n";
    }

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
