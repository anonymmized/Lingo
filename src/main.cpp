#include "achievements/AchievementsManager.hpp"
#include "core/MistakeManager.hpp"
#include "core/WordDatabase.hpp"
#include "levels/LevelManager.hpp"
#include "progress/ProgressManager.hpp"
#include "quiz/QuizGame.hpp"
#include "utils/TerminalUi.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace
{
    constexpr const char* kWordsFile =
        "assets/words.txt";
    constexpr const char* kProgressFile =
        "assets/progress.txt";
    constexpr const char* kMistakesFile =
        "assets/mistakes.txt";

    int calculateAccuracy(
        int correct,
        int total)
    {
        if (total == 0)
        {
            return 0;
        }

        return correct * 100 / total;
    }

    std::string getRoundMessage(
        int accuracy)
    {
        if (accuracy >= 90)
        {
            return "Супер! Почти без ошибок.";
        }

        if (accuracy >= 70)
        {
            return "Хороший результат. Еще чуть-чуть.";
        }

        return "Это тренировка. Следующий раунд будет лучше.";
    }

    bool saveState(
        const ProgressManager& progressManager,
        const MistakeManager& mistakeManager)
    {
        return progressManager.save(
                   kProgressFile) &&
               mistakeManager.save(
                   kMistakesFile);
    }

    void clampCurrentLevel(
        ProgressManager& progressManager,
        std::size_t levelCount)
    {
        if (levelCount == 0)
        {
            progressManager.data().currentLevel =
                1;
            return;
        }

        progressManager.data().currentLevel =
            std::clamp(
                progressManager.data().currentLevel,
                1,
                static_cast<int>(levelCount));
    }

    std::vector<Word> buildMistakeWords(
        const MistakeManager& mistakeManager,
        std::size_t count)
    {
        std::vector<Word> words;

        for (const auto& item :
             mistakeManager.topMistakes(count))
        {
            words.push_back(item.word);
        }

        return words;
    }

    void printHeader(
        const ProgressManager& progressManager,
        const LevelManager& levelManager)
    {
        TerminalUi::printBanner("LINGO");
        std::cout
            << TerminalUi::colorize(
                   TerminalUi::yellow,
                   "Текущий уровень",
                   true)
            << ": "
            << progressManager.data().currentLevel
            << "/"
            << levelManager.levelCount()
            << '\n';
        std::cout
            << TerminalUi::colorize(
                   TerminalUi::yellow,
                   "Опыт",
                   true)
            << ": "
            << progressManager.data().experience
            << '\n';
    }

    void showHelp()
    {
        TerminalUi::clearScreen();
        TerminalUi::printBanner(
            "Как играть",
            TerminalUi::green);
        std::cout
            << "1. Смотри на английское слово.\n"
            << "2. Выбирай правильный перевод цифрой.\n"
            << "3. Ошибаться можно: сложные слова попадут в тренировку.\n"
            << "4. Игру можно завершить только через пункт \"Выход\" или Ctrl+C.\n"
            << "5. Если устал, сыграй один раунд и вернись позже.\n";
        TerminalUi::waitForEnter();
    }

    void showAchievements(
        AchievementManager& achievementManager,
        const ProgressManager& progressManager)
    {
        achievementManager.update(
            progressManager.data());

        TerminalUi::clearScreen();
        TerminalUi::printBanner(
            "Достижения",
            TerminalUi::magenta);

        for (const auto& achievement :
             achievementManager.getAchievements())
        {
            std::cout
                << (achievement.unlocked
                        ? TerminalUi::colorize(
                              TerminalUi::green,
                              "[ОТКРЫТО]",
                              true)
                        : TerminalUi::colorize(
                              TerminalUi::dim,
                              "[ЗАКРЫТО]"))
                << ' '
                << achievement.title
                << " - "
                << achievement.description
                << '\n';
        }

        TerminalUi::waitForEnter();
    }

    void showStats(
        const ProgressManager& progressManager,
        const MistakeManager& mistakeManager)
    {
        TerminalUi::clearScreen();
        TerminalUi::printBanner(
            "Статистика",
            TerminalUi::cyan);

        const int totalAnswered =
            progressManager.data().totalCorrect +
            progressManager.data().totalMistakes;

        std::cout
            << "Правильных ответов: "
            << progressManager.data().totalCorrect
            << '\n';
        std::cout
            << "Ошибок: "
            << progressManager.data().totalMistakes
            << '\n';
        std::cout
            << "Точность: "
            << calculateAccuracy(
                   progressManager.data().totalCorrect,
                   totalAnswered)
            << "%\n";
        std::cout
            << "Сложных слов в копилке: "
            << mistakeManager.getWords().size()
            << "\n\n";

        std::cout
            << TerminalUi::colorize(
                   TerminalUi::red,
                   "Где ошибок больше всего:",
                   true)
            << '\n';

        const auto topMistakes =
            mistakeManager.topMistakes(5);

        if (topMistakes.empty())
        {
            std::cout
                << "Пока ошибок нет. Отличное начало.\n";
        }
        else
        {
            for (std::size_t i = 0;
                 i < topMistakes.size();
                 ++i)
            {
                std::cout
                    << i + 1
                    << ". "
                    << topMistakes[i].word.english
                    << " -> "
                    << topMistakes[i].word.russian
                    << " ("
                    << topMistakes[i].mistakes
                    << ")\n";
            }
        }

        TerminalUi::waitForEnter();
    }

    void showRoundSummary(
        const QuizResult& result)
    {
        TerminalUi::printBanner(
            "Итоги раунда",
            result.correctAnswers * 100 >=
                    result.totalQuestions * 80
                ? TerminalUi::green
                : TerminalUi::yellow);

        const int accuracy =
            calculateAccuracy(
                result.correctAnswers,
                result.totalQuestions);

        std::cout
            << "Счет: "
            << result.correctAnswers
            << "/"
            << result.totalQuestions
            << '\n';
        std::cout
            << "Точность: "
            << accuracy
            << "%\n";
        std::cout
            << getRoundMessage(accuracy)
            << "\n\n";

        bool hasMistakes = false;

        for (const auto& item :
             result.details)
        {
            if (item.correct)
            {
                continue;
            }

            if (!hasMistakes)
            {
                hasMistakes = true;
                std::cout
                    << TerminalUi::colorize(
                           TerminalUi::red,
                           "Слова для повтора:",
                           true)
                    << '\n';
            }

            std::cout
                << "- "
                << item.word.english
                << " -> "
                << item.word.russian
                << '\n';
        }

        if (!hasMistakes)
        {
            std::cout
                << TerminalUi::colorize(
                       TerminalUi::green,
                       "Ошибок нет. Так держать.",
                       true)
                << '\n';
        }

        TerminalUi::waitForEnter();
    }

    void playLevelRound(
        int levelId,
        const LevelManager& levelManager,
        const WordDatabase& database,
        QuizGame& quiz,
        ProgressManager& progressManager,
        MistakeManager& mistakeManager,
        AchievementManager& achievementManager)
    {
        TerminalUi::clearScreen();

        const auto& level =
            levelManager.getLevel(levelId);
        const int currentUnlockedLevel =
            progressManager.data().currentLevel;
        const bool advanceLevel =
            levelId == currentUnlockedLevel;

        TerminalUi::printBanner(
            "Уровень " +
            std::to_string(level.id));
        std::cout
            << "В этом раунде слов: "
            << level.words.size()
            << "\n";

        const auto result =
            quiz.play(
                level,
                database.getWords());

        progressManager.applyQuizResult(
            result,
            advanceLevel);
        clampCurrentLevel(
            progressManager,
            levelManager.levelCount());
        mistakeManager.addFromQuizResult(
            result);
        achievementManager.update(
            progressManager.data());
        saveState(
            progressManager,
            mistakeManager);

        TerminalUi::clearScreen();
        showRoundSummary(result);
    }

    void playMistakeRound(
        const WordDatabase& database,
        QuizGame& quiz,
        ProgressManager& progressManager,
        MistakeManager& mistakeManager,
        AchievementManager& achievementManager)
    {
        const auto words =
            buildMistakeWords(
                mistakeManager,
                10);

        TerminalUi::clearScreen();

        if (words.empty())
        {
            TerminalUi::printBanner(
                "Тренировка ошибок",
                TerminalUi::yellow);
            std::cout
                << "Пока нет слов для повторения.\n";
            TerminalUi::waitForEnter();
            return;
        }

        TerminalUi::printBanner(
            "Тренировка ошибок",
            TerminalUi::red);
        std::cout
            << "Здесь собраны слова, в которых было больше всего ошибок.\n";

        const auto result =
            quiz.playWords(
                words,
                database.getWords());

        progressManager.data().experience +=
            result.correctAnswers * 3;
        mistakeManager.addFromQuizResult(
            result);
        achievementManager.update(
            progressManager.data());
        saveState(
            progressManager,
            mistakeManager);

        TerminalUi::clearScreen();
        showRoundSummary(result);
    }

    int readLevelChoice(
        const LevelManager& levelManager)
    {
        TerminalUi::clearScreen();
        TerminalUi::printBanner(
            "Выбор уровня",
            TerminalUi::blue);

        for (const auto& level :
             levelManager.getLevels())
        {
            std::cout
                << level.id
                << ". Уровень "
                << level.id
                << " ("
                << level.words.size()
                << " слов)\n";
        }

        return TerminalUi::readNumber(
            "Выбери уровень: ",
            1,
            static_cast<int>(
                levelManager.levelCount()));
    }
}

int main()
{
    WordDatabase database;

    if (!database.loadFromFile(
            kWordsFile))
    {
        std::cerr
            << TerminalUi::colorize(
                   TerminalUi::red,
                   "Не удалось загрузить words.txt",
                   true)
            << '\n';
        return 1;
    }

    if (database.size() == 0)
    {
        std::cerr
            << TerminalUi::colorize(
                   TerminalUi::red,
                   "Список слов пуст.",
                   true)
            << '\n';
        return 1;
    }

    LevelManager levelManager;
    levelManager.buildLevels(
        database.getWords());

    ProgressManager progressManager;
    progressManager.load(
        kProgressFile);
    clampCurrentLevel(
        progressManager,
        levelManager.levelCount());

    MistakeManager mistakeManager;
    mistakeManager.load(
        kMistakesFile);

    AchievementManager achievementManager;
    achievementManager.initialize();
    achievementManager.update(
        progressManager.data());

    QuizGame quiz;

    while (true)
    {
        TerminalUi::clearScreen();
        printHeader(
            progressManager,
            levelManager);

        std::cout
            << '\n'
            << "1. Играть текущий уровень\n"
            << "2. Выбрать уровень\n"
            << "3. Тренировка сложных слов\n"
            << "4. Посмотреть статистику\n"
            << "5. Посмотреть достижения\n"
            << "6. Как играть\n"
            << "7. Выход\n\n";

        const int action =
            TerminalUi::readNumber(
                "Выбери пункт меню: ",
                1,
                7);

        if (action == 1)
        {
            playLevelRound(
                progressManager.data().currentLevel,
                levelManager,
                database,
                quiz,
                progressManager,
                mistakeManager,
                achievementManager);
            continue;
        }

        if (action == 2)
        {
            const int levelId =
                readLevelChoice(
                    levelManager);
            playLevelRound(
                levelId,
                levelManager,
                database,
                quiz,
                progressManager,
                mistakeManager,
                achievementManager);
            continue;
        }

        if (action == 3)
        {
            playMistakeRound(
                database,
                quiz,
                progressManager,
                mistakeManager,
                achievementManager);
            continue;
        }

        if (action == 4)
        {
            showStats(
                progressManager,
                mistakeManager);
            continue;
        }

        if (action == 5)
        {
            showAchievements(
                achievementManager,
                progressManager);
            continue;
        }

        if (action == 6)
        {
            showHelp();
            continue;
        }

        TerminalUi::clearScreen();
        std::cout
            << TerminalUi::colorize(
                   TerminalUi::green,
                   "До встречи в следующем раунде.",
                   true)
            << '\n';
        break;
    }

    return 0;
}
