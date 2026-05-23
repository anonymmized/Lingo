#include "QuizGame.hpp"

#include "../utils/Random.hpp"
#include "../utils/TerminalUi.hpp"

#include <iostream>
#include <string>
#include <unordered_set>

namespace
{
    std::string makeWordKey(const Word& word)
    {
        return word.english + '\n' +
               word.russian;
    }
}

QuizResult QuizGame::play(
    const Level& level,
    const std::vector<Word>& optionPool)
{
    return playWords(
        level.words,
        optionPool);
}

QuizResult QuizGame::playWords(
    const std::vector<Word>& words,
    const std::vector<Word>& optionPool)
{
    QuizResult result;
    const auto roundWords =
        buildRoundWords(words);

    result.totalQuestions =
        static_cast<int>(
            roundWords.size());

    int questionNumber = 1;

    for (const auto& word : roundWords)
    {
        const Question question =
            generateQuestion(
                word,
                optionPool);

        std::cout << '\n';
        TerminalUi::printDivider('-');
        std::cout
            << TerminalUi::colorize(
                   TerminalUi::yellow,
                   "Вопрос " +
                       std::to_string(
                           questionNumber) +
                       "/" +
                       std::to_string(
                           result.totalQuestions),
                   true)
            << '\n';
        TerminalUi::printProgressBar(
            questionNumber,
            result.totalQuestions);
        TerminalUi::printDivider('-');

        std::cout
            << '\n'
            << TerminalUi::colorize(
                   TerminalUi::cyan,
                   word.english,
                   true)
            << "\n\n";

        for (std::size_t i = 0;
             i < question.options.size();
             ++i)
        {
            std::cout
                << TerminalUi::colorize(
                       TerminalUi::blue,
                       std::to_string(i + 1) +
                           ".",
                       true)
                << ' '
                << question.options[i]
                << '\n';
        }

        const int selectedIndex =
            readAnswer(
                question.options.size()) -
            1;

        QuestionResult questionResult;
        questionResult.word = word;
        questionResult.selectedIndex =
            selectedIndex;
        questionResult.correctIndex =
            question.correctIndex;
        questionResult.correct =
            selectedIndex ==
            question.correctIndex;

        if (questionResult.correct)
        {
            ++result.correctAnswers;

            std::cout
                << TerminalUi::colorize(
                       TerminalUi::green,
                       "Верно!",
                       true)
                << '\n';
        }
        else
        {
            std::cout
                << TerminalUi::colorize(
                       TerminalUi::red,
                       "Пока неверно.",
                       true)
                << ' '
                << "Правильный ответ: "
                << TerminalUi::colorize(
                       TerminalUi::green,
                       question.options[
                           question.correctIndex])
                << "\n";
        }

        result.details.push_back(
            questionResult);
        ++questionNumber;
    }

    return result;
}

std::vector<Word> QuizGame::buildRoundWords(
    const std::vector<Word>& words) const
{
    std::vector<Word> roundWords;
    std::unordered_set<std::string> used;

    for (const auto& word : words)
    {
        const auto key =
            makeWordKey(word);

        if (used.contains(key))
        {
            continue;
        }

        used.insert(key);
        roundWords.push_back(word);
    }

    Random::shuffle(roundWords);
    return roundWords;
}

int QuizGame::readAnswer(
    std::size_t optionCount) const
{
    return TerminalUi::readNumber(
        TerminalUi::colorize(
            TerminalUi::magenta,
            "Твой ответ",
            true) +
            " [1-" +
            std::to_string(optionCount) +
            "]: ",
        1,
        static_cast<int>(
            optionCount));
}

Question QuizGame::generateQuestion(
    const Word& word,
    const std::vector<Word>& optionPool) const
{
    Question question;
    question.word = word;

    std::vector<std::string> options{
        word.russian};
    std::unordered_set<std::string> used{
        word.russian};

    auto candidates = optionPool;
    Random::shuffle(candidates);

    for (const auto& candidate : candidates)
    {
        if (options.size() >= 4)
        {
            break;
        }

        if (used.contains(candidate.russian))
        {
            continue;
        }

        used.insert(candidate.russian);
        options.push_back(
            candidate.russian);
    }

    Random::shuffle(options);
    question.options =
        std::move(options);

    for (std::size_t i = 0;
         i < question.options.size();
         ++i)
    {
        if (question.options[i] ==
            word.russian)
        {
            question.correctIndex =
                static_cast<int>(i);
            break;
        }
    }

    return question;
}
