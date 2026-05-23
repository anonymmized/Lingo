#pragma once

#include "Question.hpp"
#include "QuizResult.hpp"

#include "../levels/Level.hpp"

class QuizGame {
    private:
        Question generateQuestion(const Word& word, const Level& level) const;
    public:
        QuizResult play(const Level& level);
};
