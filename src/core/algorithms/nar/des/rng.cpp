#include "rng.h"

namespace algos::des {

std::random_device RNG::rd;   // only used once to initialise (seed) engine
std::mt19937 RNG::rng = std::mt19937(rd());  // random-number engine used (Mersenne-Twister in this case)
std::uniform_real_distribution<double> RNG::uni = std::uniform_real_distribution(0.0, 0.1);  // guaranteed unbiased

} // namespace algos::des