#include "raylib.h"
#include "battle.h"
#include "view_replay.h"
#include "benchmarks.h"

int main() {
    // start_timer();

    // StageData stage;
    // stage.width = 2500;
    // stage.cat_hp = stage.enemy_hp = 5000;
    // Battle<SAVE_LOGS> b(stage);

    // start_timer();
    // while (timer_ms() < 1000) {
    //     if (proc(0.05)) {
    //         b.add_cat(1);
    //     }
    //     if (proc(0.005)) {
    //         b.add_cat(7);
    //     }
    //     if (proc(0.005)) {
    //         b.add_cat(8);
    //     }

    //     if (proc(0.3)) {
    //         b.add_enemy(0);
    //     }
    //     if (proc(0.02)) {
    //         b.add_enemy(1);
    //     }
    //     b.advance();
    // }

    // std::cerr << timer_ms() << std::endl;

    // view_replay(b);

    std::cerr << benchmark_a() << std::endl;
}