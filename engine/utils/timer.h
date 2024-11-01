#ifndef TIMER_H
#define TIMER_H

#include <chrono>

struct Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> clock;

    void set() {
        clock = std::chrono::high_resolution_clock::now();
    }

    Timer() {
        set();
    }

    long long get() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - clock).count();
    }
};

#endif // TIMER_H