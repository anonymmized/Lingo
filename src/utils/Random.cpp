#include "Random.hpp"

#include <random>

namespace {
    std::mt19937& generator() {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        return gen;
    }
}

int Random::nextInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator());
}
