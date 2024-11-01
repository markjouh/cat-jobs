#ifndef VIEWER_H
#define VIEWER_H

#include "raylib.h"
#include "battle.h"

const int MARGIN = 200;
const int WINDOW_W = 960;
const int WINDOW_H = 540;

float bounce_anim(float t) {
    return pow(2, -2 * t) * abs(cos(t * 3.1415926535 * 2.5));
}

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

        time = std::clamp(time + speed, 0, b.time - 1);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(("Time: " + std::to_string(time) + "f").c_str(), 10, 10, 20, DARKGRAY);
        DrawText(("Speed: " + std::to_string(speed) + "x").c_str(), 10, 35, 20, DARKGRAY);
        DrawText(("Units: " + std::to_string(ssize(b.logs[time]))).c_str(), 10, 60, 20, DARKGRAY);
        
        DrawRectangle(0, WINDOW_H / 2.0f, WINDOW_W, WINDOW_H / 2.0f, LIGHTGRAY);

        for (auto unit : b.logs[time]) {
            if (unit.is_cat) {
                unit.pos = b.stage_width - unit.pos;
            }

            Vector2 pos = {
                (MARGIN + unit.pos) * ratio,
                WINDOW_H / 2.0f - unit.uid % 10
            };
            if (unit.knockback) {
                pos.x += ratio * (1.0f - unit.kb_frac) * (unit.is_cat ? -1 : +1) * 0.75f * KB_DIST;
                pos.y -= ratio * 75.0f * bounce_anim(unit.kb_frac);
            }

            float rad = 5e4 / b.stage_width;
            Color fill_color = unit.is_cat ? ORANGE : MAROON, line_color = BLACK;
            DrawCircleV(pos, rad, fill_color);
            DrawCircleLinesV(pos, rad, line_color);
            DrawText(std::to_string(unit.hp).c_str(), pos.x, pos.y, 15, DARKGRAY);
        }
        EndDrawing();
    }
    CloseWindow();
}

#endif // VIEWER_H