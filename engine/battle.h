#ifndef BATTLE_H
#define BATTLE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

#include "utils/defines.h"
#include "utils/arithmetic.h"
#include "utils/random.h"

#include "units.h"

struct Unit {
    int id, uid;
    int hp, pos = 0;
    int delta_hp = 0, delta_pos = 0;
    int attack_f = 0, kb_f = 0;

    Unit(int id_, int hp_) : id(id_), uid(gen_uid()), hp(hp_) {}
};

struct State {
    int time = 0;
    std::vector<Unit> cats, enemies;
};

struct Battle {
    static constexpr int KB_DIST = 165, KB_DURATION = 165;

    int stage_width = 1000;
    int cat_limit = 50, enemy_limit = 50;
    int cat_base_hp = 1000, enemy_base_hp = 2000;

    bool save_logs = false;
    
    Battle() {}

    State state;
    std::vector<State> logs;

    void add_cat(int id) {
        if (SZ(state.cats) < cat_limit) {
            state.cats.emplace_back(id, cat_data[id].hp);
        }
    }
    void add_enemy(int id) {
        if (SZ(state.enemies) < enemy_limit) {
            state.enemies.emplace_back(id, enemy_data[id].hp);
        }
    }

    void action_step(std::vector<Unit> &units, std::vector<Unit> &opp_units, const UnitData *data, const UnitData *opp_data) {
        int nxt = SZ(opp_units) - 1;
        for (Unit &unit : units) {
            if (unit.kb_f) {
                continue;
            }
            while (nxt >= 0 && (opp_units[nxt].kb_f || opp_units[nxt].pos > stage_width - unit.pos)) {
                nxt--;
            }
            const int bound = unit.pos + data[unit.id].range;
            if (nxt == -1 || stage_width - opp_units[nxt].pos > bound) {
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

                for (int i = nxt; i >= (data[unit.id].area_attack ? 0 : nxt) && stage_width - opp_units[i].pos <= bound; i--) {
                    if (!opp_units[i].kb_f) {
                        int cur = dmg;
                        if (opp_data[opp_units[i].id].strong & data[unit.id].traits) {
                            cur = floor(cur * 0.5);
                        }
                        if (opp_data[opp_units[i].id].traits & data[unit.id].strong) {
                            cur = floor(cur * 1.5);
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
        units.erase(std::remove_if(ALL(units), [&](Unit &unit) {
            unit.kb_f -= unit.kb_f > 0;

            if (cdiv(unit.hp + unit.delta_hp, data[unit.id].health_per_kb) < cdiv(unit.hp, data[unit.id].health_per_kb)) {
                unit.delta_pos -= KB_DIST;
                unit.kb_f += KB_DURATION;
                unit.attack_f = 0;
            }
            unit.hp += unit.delta_hp;
            unit.delta_hp = 0;

            unit.pos = std::clamp(unit.pos + unit.delta_pos, 0, stage_width);
            unit.delta_pos = 0;

            return unit.hp <= 0 && !unit.kb_f;
        }), units.end());
    }

    void advance() {
        auto comp = [&](const Unit &a, const Unit &b) {
            return a.pos < b.pos;
        };
        std::sort(ALL(state.cats), comp);
        std::sort(ALL(state.enemies), comp);

        action_step(state.cats, state.enemies, cat_data, enemy_data);
        action_step(state.enemies, state.cats, enemy_data, cat_data);

        resolution_step(state.cats, cat_data);
        resolution_step(state.enemies, enemy_data);

        state.time++;

        if (save_logs) {
            logs.emplace_back(state);
        }
    }
};

#endif // BATTLE_H
