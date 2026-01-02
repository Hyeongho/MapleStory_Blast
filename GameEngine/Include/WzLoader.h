#pragma once

#include "WzArchive.h"
#include <vector>
#include <memory>
#include <string>

class CWzLoader
{
public:
    bool enable(const std::string& dir);
    void disable();
    void printLoadedFiles() const;

private:
    std::vector<std::unique_ptr<CWzArchive>> archives;
};

