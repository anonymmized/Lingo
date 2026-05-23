#pragma once

#include "../core/Word.hpp"

#include <vector>

struct Level {
    int id {};
    std::vector<Word> words;
};
