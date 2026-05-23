#pragma once

#include <string>

struct Achievement {
    std::string id;
    std::string title;
    std::string description;
    bool unlocked {};
};
