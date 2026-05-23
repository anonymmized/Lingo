#include "AchievementsManager.hpp"

void AchievementManager::initialize() {
    achievements_.clear();
    achievements_.push_back({"first_level", "Первый шаг", "Пройди первый уровень", false});

    achievements_.push_back({"ten_correct", "Отлично", "Дай 10 правильных ответов", false});

    achievements_.push_back({"hundred_correct", "100 слов", "Дай 100 правильных ответов", false});

    achievements_.push_back({"fifty_mistakes_fixed", "Упорство", "Сделай 50 попыток без остановки", false});
}

void AchievementManager::update(const Progress& progress) {
    for (auto& achievement : achievements_) {
        if (achievement.id == "first_level") {
            achievement.unlocked = progress.currentLevel > 1;
        } else if (achievement.id == "ten_correct") {
            achievement.unlocked = progress.totalCorrect >= 10;
        } else if (achievement.id == "hundred_correct") {
            achievement.unlocked = progress.totalCorrect >= 100;
        } else if (achievement.id == "fifty_mistakes_fixed") {
            achievement.unlocked =
                progress.totalCorrect +
                    progress.totalMistakes >=
                50;
        }
    }
}

const std::vector<Achievement>& AchievementManager::getAchievements() const { return achievements_; }
