#include <fstream>
#include <iostream>
#include <string>

#include "raylib.h"
#include "battle.h"
#include "viewer.h"

#include "pre/unit.h"
#include "pre/relation.h"

int main() {
    // Battle b;
    // b.stage_width = 3000;
    // b.save_logs = true;

    // for (int i = 0; i < 5000; i++) {
    //     if (i % 7 == 0) {
    //         b.add_enemy(0);
    //     }
    //     if (i % 8 == 0) {
    //         b.add_cat(0);
    //     }
    //     b.advance();
    // }

    // view_replay(b);

    json cat = cat_by_name("Fishman Cat");
    cat["level"] = 30;
    apply_changes(cat);

    json enemy = enemy_by_name("Mooth");

    Relation r = make_relation(cat, enemy);
    std::cout << r.damage[0] << '\n';
}