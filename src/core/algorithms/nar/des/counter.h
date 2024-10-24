#pragma once

#include <random>
#include <iostream>

namespace algos::des {

class Counter {
private:
    static uint counter;
public:
    static void Next() {
        std::cout << counter++ << "\n";
    }
};

} // namespace algos::des
