#pragma once

#include <vector>

class Random {
    public:
        static int nextInt(int min, int max);

        template<typename T>
            static void shuffle(std::vector<T>& data);
};
