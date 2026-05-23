#pragma once

#include "MistakeWord.hpp"
#include "../quiz/QuizResult.hpp"

#include <cstddef>
#include <string>
#include <vector>

class MistakeManager {
    private:
        std::vector<MistakeWord> words_;
    public:
        void add(const Word& word);
        void addFromQuizResult(const QuizResult& result);
        std::vector<MistakeWord> topMistakes(std::size_t count) const;
        bool load(const std::string& filename);
        bool save(const std::string& filename) const;
        const std::vector<MistakeWord>& getWords() const;
};
