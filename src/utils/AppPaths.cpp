#include "AppPaths.hpp"

#include <cstdlib>
#include <vector>

namespace fs = std::filesystem;

namespace
{
    fs::path gExecutableDir;
    fs::path gCurrentDir;

    fs::path makeAbsolute(
        const fs::path& path)
    {
        std::error_code error;
        const fs::path absolutePath =
            fs::absolute(path, error);

        if (error)
        {
            return path;
        }

        return absolutePath;
    }

    fs::path appSupportDir()
    {
        if (const char* home =
                std::getenv("HOME"))
        {
            return fs::path(home) /
                   "Library" /
                   "Application Support" /
                   "Lingo";
        }

        return gCurrentDir / ".lingo";
    }
}

void AppPaths::initialize(
    const char* argv0)
{
    gCurrentDir = fs::current_path();

    if (argv0 != nullptr &&
        *argv0 != '\0')
    {
        gExecutableDir =
            makeAbsolute(argv0).parent_path();
    }

    if (gExecutableDir.empty())
    {
        gExecutableDir = gCurrentDir;
    }
}

fs::path AppPaths::resourceFile(
    const std::string& filename)
{
    const std::vector<fs::path> candidates = {
        gCurrentDir / "assets" / filename,
        gExecutableDir / "assets" / filename,
        gExecutableDir.parent_path() /
            "assets" / filename,
        gExecutableDir.parent_path() /
            "Resources" / "assets" / filename
    };

    for (const auto& candidate :
         candidates)
    {
        if (fs::exists(candidate))
        {
            return candidate;
        }
    }

    return candidates.front();
}

fs::path AppPaths::dataFile(
    const std::string& filename)
{
    const fs::path dataDir =
        appSupportDir();

    std::error_code error;
    fs::create_directories(
        dataDir,
        error);

    const fs::path target =
        dataDir / filename;

    if (fs::exists(target))
    {
        return target;
    }

    const std::vector<fs::path> legacyCandidates = {
        gCurrentDir / "assets" / filename,
        gExecutableDir / "assets" / filename,
        gExecutableDir.parent_path() /
            "assets" / filename
    };

    for (const auto& legacy :
         legacyCandidates)
    {
        if (fs::exists(legacy))
        {
            return legacy;
        }
    }

    return target;
}
