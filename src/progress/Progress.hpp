#pragma once

#include <vector>

struct Progress {
    int currentLevel {1};
    int experience {};
    int totalCorrect {};
    int totalMistakes {};
    std::vector<int> completedLevels;
};


