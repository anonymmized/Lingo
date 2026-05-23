#include "WordDatabase.hpp"

#include <fstream>
#include <algorithm>
#include <sstream>

namespace
{
    std::string trim(std::string value)
    {
        const auto first =
            value.find_first_not_of(
                " \t\r\n");

        if (first == std::string::npos)
        {
            return {};
        }

        const auto last =
            value.find_last_not_of(
                " \t\r\n");

        return value.substr(
            first,
            last - first + 1);
    }
}

bool WordDatabase::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    words_.clear();

    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::string english;
        std::string russian;

        const auto separator =
            line.find(" - ");

        if (separator != std::string::npos) {
            english =
                trim(line.substr(0, separator));
            russian =
                trim(line.substr(separator + 3));
        } else {
            std::stringstream ss(line);

            if (std::getline(ss, english, ';') &&
                std::getline(ss, russian)) {
                english = trim(english);
                russian = trim(russian);
            }
        }

        if (!english.empty() &&
            !russian.empty()) {
            words_.push_back({english, russian});
        }
    }
    return true;
}

const std::vector<Word>& WordDatabase::getWords() const { return words_;}

std::size_t WordDatabase::size() const { return words_.size(); }
