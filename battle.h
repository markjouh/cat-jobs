#ifndef BATTLE_H
#define BATTLE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "common.h"
#include "units.h"
#include "data.h"

const int UID_RANGE = 10000;
const int LAYERS = 10;

struct Unit {
    int id, uid;
    int hp, pos = 0;
    int delta_hp = 0, delta_pos = 0;
    int attack_f = 0, kb_f = 0;

    Unit(int id_, int hp_) : id(id_), uid(rand() % UID_RANGE), hp(hp_) {}
};

struct UnitState {
    bool is_cat;
    int id, uid;
    int hp, pos;
    bool knockback;
    float kb_frac;

    UnitState(bool c, const Unit &u) : is_cat(c), id(u.id), uid(u.uid),
        hp(u.hp), pos(u.pos), knockback(u.kb_f > 0),
        kb_frac(float(KB_DURATION - u.kb_f) / KB_DURATION) {}
};

struct StageData {
    int width;
    int cat_hp, enemy_hp;
};

const int SAVE_LOGS = 1;
const int CAT_LIMIT = 50, ENEMY_LIMIT = 100;

template <int OPT = 0>
struct Battle {
    StageData stage;

    int time = 0;
    std::vector<Unit> cats, enemies;
    std::vector<std::vector<UnitState>> logs;

    void add_cat(int id) {
        if (ssize(cats) < CAT_LIMIT) {
            cats.emplace_back(id, cat_data[id].hp);
        }
    }
    void add_enemy(int id) {
        if (ssize(enemies) < ENEMY_LIMIT) {
            enemies.emplace_back(id, enemy_data[id].hp);
        }
    }

    Battle(StageData s) : stage(s) {
        cats.reserve(CAT_LIMIT);
        enemies.reserve(ENEMY_LIMIT);
    }

    void action_step(std::vector<Unit> &units, std::vector<Unit> &opp_units, const UnitData *data, const UnitData *opp_data) {
        int nxt = ssize(opp_units) - 1;
        for (Unit &unit : units) {
            if (unit.kb_f) {
                continue;
            }
            while (nxt >= 0 && (opp_units[nxt].kb_f || opp_units[nxt].pos > stage.width - unit.pos)) {
                nxt--;
            }
            const int bound = unit.pos + data[unit.id].range;
            if (nxt == -1 || stage.width - opp_units[nxt].pos > bound) {
                unit.delta_pos += data[unit.id].speed;
            } else {
                if (++unit.attack_f == data[unit.id].attack_period) {
                    unit.attack_f = 0;
                }

                int dmg = -1;
                for (int i = 0; i < 3; i++) {
                    if (unit.attack_f == data[unit.id].hit_f[i]) {
                        dmg = data[unit.id].damage[i];
                        break;
                    }
                }
                if (dmg == -1) {
                    continue;
                }

                for (int i = nxt; i >= (data[unit.id].area_attack ? 0 : nxt) && stage.width - opp_units[i].pos <= bound; i--) {
                    if (!opp_units[i].kb_f) {
                        int cur = dmg;
                        if (opp_data[opp_units[i].id].strong & data[unit.id].traits) {
                            cur /= 2;
                        }
                        if (opp_data[opp_units[i].id].traits & data[unit.id].strong) {
                            cur = (cur * 3) / 2;
                        }
                        if (opp_data[opp_units[i].id].traits & data[unit.id].massive) {
                            cur *= 3;
                        }
                        if (opp_data[opp_units[i].id].traits & data[unit.id].insane) {
                            cur *= 5;
                        }
                        opp_units[i].delta_hp -= cur;
                    }
                }
            }
        }
    }

    void resolution_step(std::vector<Unit> &units, const UnitData *data) {
        units.erase(std::remove_if(all(units), [&](Unit &unit) {
            unit.kb_f -= unit.kb_f > 0;

            if (cdiv(unit.hp + unit.delta_hp, data[unit.id].health_per_kb) < cdiv(unit.hp, data[unit.id].health_per_kb)) {
                unit.delta_pos -= KB_DIST;
                unit.kb_f += KB_DURATION;
                unit.attack_f = 0;
            }
            unit.hp += unit.delta_hp;
            unit.delta_hp = 0;

            unit.pos = std::clamp(unit.pos + unit.delta_pos, 0, stage.width);
            unit.delta_pos = 0;

            return unit.hp <= 0 && !unit.kb_f;
        }), units.end());
    }

    void save_logs() {
        logs.emplace_back();
        for (const Unit &unit : cats) {
            logs.back().emplace_back(1, unit);
        }
        for (const Unit &unit : enemies) {
            logs.back().emplace_back(0, unit);
        }
        std::sort(all(logs.back()), [&](const UnitState &a, const UnitState &b) {
            if (a.uid % LAYERS != b.uid % LAYERS) {
                return a.uid % LAYERS > b.uid % LAYERS;
            }
            return a.uid < b.uid;
        });
    }

    void advance() {
        auto comp = [&](const Unit &a, const Unit &b) {
            return a.pos < b.pos;
        };
        std::sort(all(cats), comp);
        std::sort(all(enemies), comp);

        action_step(cats, enemies, cat_data, enemy_data);
        action_step(enemies, cats, enemy_data, cat_data);

        resolution_step(cats, cat_data);
        resolution_step(enemies, enemy_data);

        if constexpr (OPT & SAVE_LOGS) {
            save_logs();
        }

        time++;
    }
};

#endif // BATTLE_H
