#pragma once

#include "../core/Word.hpp"

#include <vector>

struct QuestionResult {
    Word word;
    bool correct {};
    int selectedIndex {};
    int correctIndex {};
};

struct QuizResult {
    int totalQuestions {};
    int correctAnswers {};
    std::vector<QuestionResult> details;
};
