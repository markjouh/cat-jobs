#ifndef COMMON_H
#define COMMON_H

#include <random>
#include <chrono>

#define all(x) std::begin(x), std::end(x)

constexpr int cdiv(int a, int b) {
    return (a + b - 1) / b;
}

bool proc(float p) {
    return float(rand()) / RAND_MAX < p;
}

std::chrono::time_point<std::chrono::high_resolution_clock> timer;

void start_timer() {
    timer = std::chrono::high_resolution_clock::now();
}
long long timer_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timer).count();
}

#endif // COMMON_H