#pragma once

#include <filesystem>
#include <string>

namespace AppPaths
{
    void initialize(const char* argv0);

    std::filesystem::path resourceFile(
        const std::string& filename);

    std::filesystem::path dataFile(
        const std::string& filename);
}
