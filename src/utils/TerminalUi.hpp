#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>

namespace TerminalUi
{
    inline constexpr std::string_view reset = "\033[0m";
    inline constexpr std::string_view bold = "\033[1m";
    inline constexpr std::string_view dim = "\033[2m";

    inline constexpr std::string_view red = "\033[31m";
    inline constexpr std::string_view green = "\033[32m";
    inline constexpr std::string_view yellow = "\033[33m";
    inline constexpr std::string_view blue = "\033[34m";
    inline constexpr std::string_view magenta = "\033[35m";
    inline constexpr std::string_view cyan = "\033[36m";

    inline std::string colorize(
        std::string_view color,
        std::string_view text,
        bool useBold = false)
    {
        std::string value;

        if (useBold)
        {
            value += bold;
        }

        value += color;
        value += text;
        value += reset;

        return value;
    }

    inline void printDivider(
        char fill = '=',
        int width = 44)
    {
        std::cout
            << std::string(
                   std::max(0, width),
                   fill)
            << '\n';
    }

    inline void printBanner(
        std::string_view title,
        std::string_view color = cyan)
    {
        printDivider('=');
        std::cout
            << colorize(
                   color,
                   std::string(title),
                   true)
            << '\n';
        printDivider('=');
    }

    inline void printProgressBar(
        int current,
        int total,
        int width = 24)
    {
        const int safeTotal =
            std::max(total, 1);

        const double ratio =
            std::clamp(
                static_cast<double>(current) /
                    safeTotal,
                0.0,
                1.0);

        const int filled =
            static_cast<int>(ratio * width);

        std::cout << '[';

        for (int i = 0; i < width; ++i)
        {
            std::cout
                << (i < filled ? '#' : '.');
        }

        std::cout
            << "] "
            << current
            << '/'
            << total
            << '\n';
    }

    inline void clearScreen()
    {
        std::cout
            << "\033[2J\033[H";
    }

    inline int readNumber(
        std::string_view prompt,
        int minValue,
        int maxValue)
    {
        int value;

        while (true)
        {
            std::cout << prompt;

            if (std::cin >> value &&
                value >= minValue &&
                value <= maxValue)
            {
                std::cin.ignore(
                    std::numeric_limits<
                        std::streamsize>::max(),
                    '\n');
                return value;
            }

            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<
                    std::streamsize>::max(),
                '\n');

            std::cout
                << colorize(
                       red,
                       "Нужно ввести число из меню.")
                << '\n';
        }
    }

    inline void waitForEnter()
    {
        std::cout
            << '\n'
            << colorize(
                   dim,
                   "Нажми Enter, чтобы продолжить...")
            << std::flush;

        std::cin.ignore(
            std::numeric_limits<
                std::streamsize>::max(),
            '\n');
    }
}
