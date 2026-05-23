#include "MistakeManager.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

void MistakeManager::add(const Word& word) {
    auto it = std::find_if(words_.begin(), words_.end(), [&](const MistakeWord& item) {
            return item.word.english == word.english;
        });
    if (it != words_.end()) {
        ++it->mistakes;
        return;
    }

    words_.push_back({word, 1});
}

bool MistakeManager::load(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) return false;

    words_.clear();

    std::string english;
    std::string russian;
    int mistakes;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        const auto firstSeparator =
            line.find(';');
        const auto secondSeparator =
            line.rfind(';');

        if (firstSeparator == std::string::npos ||
            secondSeparator == std::string::npos ||
            firstSeparator == secondSeparator) {
            continue;
        }

        english =
            line.substr(0, firstSeparator);
        russian =
            line.substr(
                firstSeparator + 1,
                secondSeparator - firstSeparator - 1);

        std::stringstream mistakesStream(
            line.substr(secondSeparator + 1));

        if (mistakesStream >> mistakes) {
            words_.push_back(
                {{english, russian}, mistakes});
        }
    }

    return true;
}

void MistakeManager::addFromQuizResult(const QuizResult& result) {
    for (const auto& item : result.details) {
        if (!item.correct) add(item.word);
    }
}

std::vector<MistakeWord> MistakeManager::topMistakes(std::size_t count) const {
    auto copy = words_;

    std::sort(copy.begin(), copy.end(), [](const MistakeWord& lhs, const MistakeWord& rhs) {
                return lhs.mistakes > rhs.mistakes;
            });
    if (copy.size() > count) copy.resize(count);

    return copy;
}

bool MistakeManager::save(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    for (const auto& word : words_) {
        file
            << word.word.english
            << ';'
            << word.word.russian
            << ';'
            << word.mistakes
            << '\n';
    }
    return true;
}

const std::vector<MistakeWord>& MistakeManager::getWords() const { return words_; }
