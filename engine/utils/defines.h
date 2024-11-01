#ifndef DEFS_H
#define DEFS_H

#include <cstdint>

#define ALL(x) (x).begin(), (x).end()

#define BIT(x) (1 << (x))
#define MASK(x) (BIT(x) - 1)

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

#endif // DEFS_H