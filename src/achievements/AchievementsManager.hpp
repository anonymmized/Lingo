#pragma once

#include "Achievements.hpp"
#include "../progress/Progress.hpp"

#include <vector>

class AchievementManager {
    private:
        std::vector<Achievement> achievements_;
    public:
        void initialize();
        void update(const Progress& progress);
        const std::vector<Achievement>& getAchievements() const;
};
