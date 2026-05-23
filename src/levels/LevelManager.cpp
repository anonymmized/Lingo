#include "LevelManager.hpp"

#include <stdexcept>

void LevelManager::buildLevels(const std::vector<Word>& words, std::size_t wordsPerLevel) {
    levels_.clear();

    if (wordsPerLevel == 0) return;

    int levelId = 1;

    for (std::size_t i = 0; i < words.size(); i += wordsPerLevel) {
        Level level;
        level.id = levelId++;

        const std::size_t end = std::min(i + wordsPerLevel, words.size());

        for (std::size_t j = i; j < end; ++j) {
            level.words.push_back(words[j]);
        }

        levels_.push_back(std::move(level));
    }
}

const Level& LevelManager::getLevel(int id) const {
    if (id < 1 || static_cast<std::size_t>(id) > levels_.size())
        throw std::out_of_range("Level not found");
    return levels_[id-1];
}

const std::vector<Level>& LevelManager::getLevels() const { return levels_; }

std::size_t LevelManager::levelCount() const { return levels_.size(); }
