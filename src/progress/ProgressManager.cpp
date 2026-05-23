#include "ProgressManager.hpp"

#include <fstream>

bool ProgressManager::load(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) return false;

    progress_ = {};

    file >> progress_.currentLevel;
    file >> progress_.experience;
    file >> progress_.totalCorrect;
    file >> progress_.totalMistakes;

    return true;
}

void ProgressManager::applyQuizResult(const QuizResult& result, bool advanceLevel) {
    progress_.totalCorrect += result.correctAnswers;
    progress_.totalMistakes += result.totalQuestions - result.correctAnswers;

    if (result.totalQuestions == 0) {
        return;
    }

    const double score =
        static_cast<double>(result.correctAnswers) /
        result.totalQuestions;

    if (score >= 0.8) {
        if (advanceLevel) {
            ++progress_.currentLevel;
        }
        progress_.experience += result.correctAnswers * 10;
    } else {
        progress_.experience += result.correctAnswers * 5;
    }
}

bool ProgressManager::save(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) return false;

    file << progress_.currentLevel << '\n';
    file << progress_.experience << '\n';
    file << progress_.totalCorrect << '\n';
    file << progress_.totalMistakes << '\n';

    return true;
}

Progress& ProgressManager::data() { return progress_; }

const Progress& ProgressManager::data() const { return progress_; }
