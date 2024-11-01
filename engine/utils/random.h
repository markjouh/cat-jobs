#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>
#include <random>

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

int gen_uid() {
    return xorshift32() % 10000;
}

#endif // RANDOM_H