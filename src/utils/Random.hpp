#pragma once

#include <algorithm>
#include <random>
#include <vector>

class Random {
    public:
        static int nextInt(int min, int max);

        template<typename T>
        static void shuffle(std::vector<T>& data) {
            static std::mt19937 gen(std::random_device{}());
            std::shuffle(data.begin(), data.end(), gen);
        }
};
