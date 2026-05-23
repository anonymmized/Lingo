#pragma once

#include "Level.hpp"

#include <vector>

class LevelManager {
    private:
        std::vector<Level> levels_;
    public:
        void buildLevels(const std::vector<Word>& words, std::size_t wordsPerLevel = 10);
        const Level& getLevel(int id) const;
        const std::vector<Level>& getLevels() const;
        std::size_t levelCount() const;
};
