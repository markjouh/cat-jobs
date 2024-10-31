#ifndef UNITS_H
#define UNITS_H

#include <cstdint>

#include "common.h"

const uint16_t TRAITLESS = 1 << 0;
const uint16_t RED_T = 1 << 1;
const uint16_t FLOATING = 1 << 2;
const uint16_t BLACK_T = 1 << 3;
const uint16_t METAL = 1 << 4;
const uint16_t ANGEL = 1 << 5;
const uint16_t ALIEN = 1 << 6;
const uint16_t ZOMBIE = 1 << 7;
const uint16_t AKU = 1 << 8;
const uint16_t RELIC = 1 << 9;
const uint16_t COLOSSUS = 1 << 10;
const uint16_t BEHEMOTH = 1 << 11;
const uint16_t SAGE = 1 << 12;
const uint16_t TYPELESS = 1 << 13;
const uint16_t ALL_TRAITS = (1 << 14) - 1;
const uint16_t TRAITED = ALL_TRAITS ^ TRAITLESS;

const uint16_t WEAKEN = 1 << 0;
const uint16_t FREEZE = 1 << 1;
const uint16_t SLOW = 1 << 2;
const uint16_t KNOCKBACK = 1 << 3;
const uint16_t WAVE = 1 << 4;
const uint16_t SURGE = 1 << 5;
const uint16_t WARP = 1 << 6;
const uint16_t CURSE = 1 << 7;
const uint16_t TOXIC = 1 << 8;
const uint16_t SHOCKWAVE = 1 << 9;
const uint16_t ALL_IMMUNITIES = (1 << 10) - 1;

struct UnitData {
    const int id;
    const char* name = "Unnamed unit";

    const uint16_t traits = 0, immunities = 0;
    const uint16_t strong = 0, massive = 0, insane = 0;

    const int range;
    const bool area_attack = false;
    const int hits = 1;
    const int attack_period;
    const int hit_f[3], damage[3];

    const int hp, kbs;
    const int speed;

    const int health_per_kb = cdiv(hp, kbs);
};

constexpr UnitData cat_data[9] = {
    {
        .name = "Cat",
        .range = 140, .attack_period = 37, .hit_f = {8, -1, -1}, .damage = {8, 0, 0},
        .hp = 100, .kbs = 3, .speed = 10
    },
    {
        .name = "Tank Cat",
        .range = 110, .area_attack = 1, .attack_period = 67, .hit_f = {8, -1, -1}, .damage = {2, 0, 0},
        .hp = 400, .kbs = 1, .speed = 8
    },
    {
        .name = "Axe Cat",
        .strong = RED_T,
        .range = 150, .attack_period = 27, .hit_f = {8, -1, -1}, .damage = {25, 0, 0},
        .hp = 200, .kbs = 3, .speed = 12
    },
    {
        .name = "Gross Cat",
        .range = 350, .attack_period = 127, .hit_f = {8, -1, -1}, .damage = {100, 0, 0},
        .hp = 400, .kbs = 3, .speed = 10
    },
    {
        .name = "Cow Cat",
        .range = 140, .attack_period = 10, .hit_f = {6, -1, -1}, .damage = {13, 0, 0},
        .hp = 500, .kbs = 5, .speed = 30
    },
    {
        .name = "Bird Cat",
        .range = 170, .area_attack = 1, .attack_period = 49, .hit_f = {10, -1, -1}, .damage = {140, 0, 0},
        .hp = 300, .kbs = 4, .speed = 10
    },
    {
        .name = "Fish Cat",
        .range = 150, .attack_period = 53, .hit_f = {10, -1, -1}, .damage = {180, 0, 0},
        .hp = 700, .kbs = 3, .speed = 10
    },
    {
        .name = "Lizard Cat",
        .range = 400, .attack_period = 129, .hit_f = {10, -1, -1}, .damage = {350, 0, 0},
        .hp = 800, .kbs = 3, .speed = 10
    },
    {
        .name = "Titan Cat",
        .range = 150, .area_attack = 1, .attack_period = 67, .hit_f = {18, -1, -1}, .damage = {280, 0, 0},
        .hp = 1000, .kbs = 1, .speed = 8
    }
};

constexpr UnitData enemy_data[2] = {
    {
        .name = "Doge",
        .traits = TRAITLESS,
        .range = 110, .attack_period = 47, .hit_f = {8, 0, 0}, .damage = {8, 0, 0},
        .hp = 90, .kbs = 3, .speed = 5
    },
    {
        .name = "Peng",
        .traits = TRAITLESS,
        .range = 150, .attack_period = 24, .hit_f = {8, 0, 0}, .damage = {80, 0, 0},
        .hp = 1300, .kbs = 3, .speed = 11
    }
};

#endif // UNITS_H