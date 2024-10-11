#pragma once

#include <random>

namespace algos::des {

class RNG {
private:
    static std::random_device rd;   // only used once to initialise (seed) engine
    static std::mt19937 rng;  // random-number engine used (Mersenne-Twister in this case)
    static std::uniform_real_distribution<double> uni;  // guaranteed unbiased
    bool initialized;
public:
    static double Next() {
        return uni(rng);
    }
};


} // namespace algos::des