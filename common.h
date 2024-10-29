#ifndef COMMON_H
#define COMMON_H

#include <random>
#include <chrono>

#define all(x) std::begin(x), std::end(x)

constexpr int cdiv(int a, int b) {
    return (a + b - 1) / b;
}

uint32_t xorshift32_state = rand();

uint32_t xorshift32() {
    int32_t x = xorshift32_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return xorshift32_state = x;
}

bool proc(float p) {
    return xorshift32() / 4294967296.0f < p;
}

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

std::chrono::time_point<std::chrono::high_resolution_clock> timer;

void start_timer() {
    timer = std::chrono::high_resolution_clock::now();
}
long long timer_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timer).count();
}

/*
Sources:
https://github.com/battlecatsultimate/BCU_java_util_common/blob/1ed5b8d261cd3725b2d69eb4cfb691c9fa31c737/util/Data.java

Notes:
BCU lists the default knockback time as 11, which conflicts with the wiki's 12.
It's not clear to me whether we're supposed to count the activation frame, which
is probably the reason for this difference.
*/

// Knockbacks from going under an HP cutoff
const int KB_DIST = 165, KB_DURATION = 12;
// Knockbacks from ability procs and sniper the cat (small)
const int PROC_KB_DIST = 345, PROC_KB_DURATION = 12;
// Knockbacks from boss shockwaves (large)
const int BOSS_KB_DIST = 705, BOSS_KB_DURATION = 47;

const int WINDOW_W = 960;
const int WINDOW_H = 540;

#endif // COMMON_H