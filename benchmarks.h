#ifndef BENCHMARKS_H
#define BENCHMARKS_H

#include "common.h"
#include "battle.h"

int benchmark_a() {
    srand(time(0));

    int res = 0;
    for (int i = 0; i < 5; i++) {
        StageData stage;
        stage.width = 1000;
        stage.cat_hp = stage.enemy_hp = 5000;
        Battle b(stage);

        start_timer();
        while (timer_ms() < 1000) {
            if (proc(0.05)) {
                b.add_cat(1);
            }
            if (proc(0.005)) {
                b.add_cat(7);
            }
            if (proc(0.005)) {
                b.add_cat(8);
            }

            if (proc(0.3)) {
                b.add_enemy(0);
            }
            if (proc(0.01)) {
                b.add_enemy(1);
            }
            b.advance();
        }
        res += b.time;
    }
    return res / 5;
}

#endif // BENCHMARKS_H