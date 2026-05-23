#pragma once

#include "Progress.hpp"

#include <string>

class ProgressManager {
    private:
        Progress progress_;
    public:
        bool load(const std::string& filename);
        bool save(const std::string& filename) const;
        Progress& data();
        const Progress& data() const;
};
