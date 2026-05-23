#pragma once

#include "Word.hpp"

#include <string>
#include <vector>

class WordDatabase {
    private:
        std::vector<Word> words_;
    public:
        bool loadFromFile(const std::string& filename);

        const std::vector<Word>& getWords() const;

        std::size_t size() const;
};
