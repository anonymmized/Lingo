#pragma once

#include "MistakeWord.hpp"

#include <string>
#include <vector>

class MistakeManager {
    private:
        std::vector<MistakeWord> words_;
    public:
        void add(const Word& word);
        bool load(const std::string filename);
        bool save(const std::string& filename) const;
        const std::vector<MistakeWord>& getWords() const;
};
