#ifndef VIEWER_H
#define VIEWER_H

#include "raylib.h"
#include "battle.h"

const int MARGIN = 200;
const int WINDOW_W = 960;
const int WINDOW_H = 540;

void view_replay(const auto &b) {
    InitWindow(WINDOW_W, WINDOW_H, "CatJobs v0.1");
    SetTargetFPS(60);

    // Coefficient for game coordinates -> pixels on window
    const float ratio = float(WINDOW_W) / (b.stage_width + 2 * MARGIN);

    int speed = 1;

    int time = 0;
    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_LEFT)) {
            speed -= speed > 1;
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            speed++;
        }

        time = std::clamp(time + speed, 0, SZ(b.logs) - 1);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(("Time: " + std::to_string(time) + "f").c_str(), 10, 10, 20, DARKGRAY);
        DrawText(("Speed: " + std::to_string(speed) + "x").c_str(), 10, 35, 20, DARKGRAY);
        DrawText(("Units: " + std::to_string(SZ(b.logs[time].cats) + SZ(b.logs[time].enemies))).c_str(), 10, 60, 20, DARKGRAY);
        
        DrawRectangle(0, WINDOW_H / 2.0f, WINDOW_W, WINDOW_H / 2.0f, LIGHTGRAY);

        auto draw_unit = [&](Unit unit) {
            Vector2 pos = {
                (MARGIN + unit.pos) * ratio,
                WINDOW_H / 2.0f - unit.uid % 10
            };
            if (unit.kb_f > 0) {
                pos.y -= 100.0f * ratio;
            }

            float rad = 5e4 / b.stage_width;
            DrawCircleV(pos, rad, MAROON);
            DrawCircleLinesV(pos, rad, BLACK);
            DrawText(std::to_string(unit.hp).c_str(), pos.x, pos.y, 15, DARKGRAY);
        };

        for (Unit cat : b.logs[time].cats) {
            cat.pos = b.stage_width - cat.pos;
            draw_unit(cat);
        }
        for (Unit enemy : b.logs[time].enemies) {
            draw_unit(enemy);
        }

        EndDrawing();
    }
    CloseWindow();
}

#endif // VIEWER_H