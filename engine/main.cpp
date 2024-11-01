#include <atomic>
#include <thread>

#include "raylib.h"
#include "battle.h"
#include "view_replay.h"

int main() {
    Battle b;
    b.stage_width = 5000;
    b.save_logs = true;

    for (int i = 0; i < 5000; i++) {
        if (i % 7 == 0) {
            b.add_enemy(0);
        }
        if (i % 8 == 0) {
            b.add_cat(0);
        }
        b.advance();
    }

    view_replay(b);
}