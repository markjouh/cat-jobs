#ifndef UNITS_H
#define UNITS_H

#include <cstdint>

#include "utils/defines.h"
#include "utils/arithmetic.h"

const u16 TRAITLESS = BIT(0);
const u16 RED_T = BIT(1);
const u16 FLOATING = BIT(2);
const u16 BLACK_T = BIT(3);
const u16 METAL = BIT(4);
const u16 ANGEL = BIT(5);
const u16 ALIEN = BIT(6);
const u16 ZOMBIE = BIT(7);
const u16 AKU = BIT(8);
const u16 RELIC = BIT(9);
const u16 COLOSSUS = BIT(10);
const u16 BEHEMOTH = BIT(11);
const u16 SAGE = BIT(12);
const u16 TYPELESS = BIT(13);
const u16 ALL_TRAITS = MASK(14);
const u16 TRAITED = ALL_TRAITS ^ TRAITLESS;

const u16 WEAKEN = BIT(0);
const u16 FREEZE = BIT(1);
const u16 SLOW = BIT(2);
const u16 KNOCKBACK = BIT(3);
const u16 WAVE = BIT(4);
const u16 SURGE = BIT(5);
const u16 WARP = BIT(6);
const u16 CURSE = BIT(7);
const u16 TOXIC = BIT(8);
const u16 SHOCKWAVE = BIT(9);
const u16 ALL_IMMUNITIES = BIT(10);

struct UnitData {
    const int id;
    const char* name = "Unnamed unit";

    const u16 traits = 0, immunities = 0;
    const u16 strong = 0, massive = 0, insane = 0;

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