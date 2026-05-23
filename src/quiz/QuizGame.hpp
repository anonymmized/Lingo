#pragma once

#include "Question.hpp"
#include "QuizResult.hpp"

#include "../levels/Level.hpp"

class QuizGame {
    private:
        std::vector<Word> buildRoundWords(const std::vector<Word>& words) const;
        Question generateQuestion(const Word& word, const std::vector<Word>& optionPool) const;
        int readAnswer(std::size_t optionCount) const;
    public:
        QuizResult play(const Level& level, const std::vector<Word>& optionPool);
        QuizResult playWords(const std::vector<Word>& words, const std::vector<Word>& optionPool);
};
