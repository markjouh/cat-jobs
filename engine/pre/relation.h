#ifndef RELATION_H
#define RELATION_H

#include "json.hpp"

using json = nlohmann::json;

#include "../utils/defines.h"

struct Relation {
    int damage[3];
};

Relation make_relation(json from, json to) {
    float dmg_coeff = 1.0;

    if (from.contains("massive_damage") && to.contains("traits")) {
        bool flag = false;
        for (auto s : from["massive_damage"]) {
            for (auto t : to["traits"]) {
                flag |= t == s;
            }
        }
        if (flag) {
            dmg_coeff *= 3;
        }
    }

    if (from.contains("insane_damage") && to.contains("traits")) {
        bool flag = false;
        for (auto s : from["insane_damage"]) {
            for (auto t : to["traits"]) {
                flag |= t == s;
            }
        }
        if (flag) {
            dmg_coeff *= 5;
        }
    }

    Relation res;
    for (int i = 0; i < SZ(from["hit_damages"]); i++) {
        res.damage[i] = round(dmg_coeff * int(from["hit_damages"][i]));
    }
    return res;
}

#endif // RELATION_H
