#pragma once

#include "Progress.hpp"
#include "../quiz/QuizResult.hpp"

#include <string>

class ProgressManager {
    private:
        Progress progress_;
    public:
        bool load(const std::string& filename);
        void applyQuizResult(const QuizResult& result, bool advanceLevel = true);
        bool save(const std::string& filename) const;
        Progress& data();
        const Progress& data() const;
};
