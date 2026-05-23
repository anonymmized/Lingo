#pragma once

#include "../core/Word.hpp"

#include <string>
#include <vector>

struct Question {
    Word word;
    std::vector<std::string> options;
    int correctIndex {};
};
