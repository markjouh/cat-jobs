#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <cstdint>
#include <random>

constexpr int cdiv(int a, int b) {
    return (a + b - 1) / b;
}

#endif // ARITHMETIC_H