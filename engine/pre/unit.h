#ifndef UNIT_H
#define UNIT_H

#include <iostream>

#include "json.hpp"

using json = nlohmann::json;

#include "../utils/defines.h"

json db_cats = json::parse(std::ifstream("../data/parsed/cats.json"));
json db_enemies = json::parse(std::ifstream("../data/parsed/enemies.json"));

json cat_by_id(int id) {
    return db_cats[id];
}
json cat_by_name(std::string name) {
    for (auto cat : db_cats) {
        for (auto form : cat) {
            if (std::string(form["name"]) == name) {
                return form;
            }
        }
    }
    std::cerr << "Error in cat_by_name(): No cat with name \"" << name << "\" found." << std::endl;
    exit(1);
};

json enemy_by_id(int id) {
    return db_enemies[id];
}
json enemy_by_name(std::string name) {
    for (auto enemy : db_enemies) {
        if (std::string(enemy["name"]) == name) {
            return enemy;
        }
    }
    std::cerr << "Error in enemy_by_name(): No enemy with name \"" << name << "\" found." << std::endl;
    exit(1);
};

json magnify_unit(json unit, float coeff) {
    if (!unit.contains("attack") || !unit.contains("health")) {
        std::cerr << "Error in magnify_unit(): invalid unit data." << std::endl;
        exit(1);
    }

    unit["attack"] = round(coeff * int(unit["attack"]));
    unit["health"] = round(coeff * int(unit["health"]));
    if (unit.contains("hit_damages")) {
        for (auto &x : unit["hit_damages"]) {
            x = round(coeff * int(x));
        }
    }
    return unit;
}

void apply_changes(json &unit) {
    float magnify = 1.0f;

    if (unit.contains("level")) {
        magnify += 0.2f * (int(unit["level"]) - 1);
    }
    if (unit.contains("magnification")) {
        magnify *= int(unit["magnification"]);
    }

    unit = magnify_unit(unit, magnify);
}

#endif // UNIT_H
