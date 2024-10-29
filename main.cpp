#include <atomic>
#include <thread>

#include "raylib.h"
#include "battle.h"
#include "view_replay.h"
#include "benchmarks.h"

#include "common.h"

Texture2D catjobs;

Color DARKORANGE = Color{240, 101, 67, 255};
Color CLOUD = Color{215, 221, 224, 255};
Color DARKCLOUD = Color{143, 147, 156, 255};

int frame = 0, screen = 0;
bool transition = false;
int t_frame = 0, nxt_screen;

void handle_transition() {
    static constexpr int FRAMES_IN = 60, FRAMES_OUT = 60;

    auto door_in = [](float t) -> float {
        return 1.0f - pow(2, -10 * t) * abs(cos(t * 3.1415926535 * 3));
    };
    auto door_out = [](float t) -> float {
        return 1.0f - pow((t + 0.4), 6);
    };

    if (transition) {
        t_frame++;
        if (t_frame < FRAMES_IN + FRAMES_OUT) {
            const int w = 1 + (t_frame < FRAMES_IN ? door_in(float(t_frame) / FRAMES_IN) : door_out(float(t_frame - FRAMES_IN) / FRAMES_OUT)) * WINDOW_W / 2;
            DrawRectangle(0, 0, w, WINDOW_H, CLOUD);
            DrawRectangle(w, 0, 2, WINDOW_H, DARKCLOUD);
            DrawRectangle(WINDOW_W - w, 0, WINDOW_W / 2, WINDOW_H, CLOUD);
            DrawRectangle(WINDOW_W - w, 0, 2, WINDOW_H, DARKCLOUD);
        } else {
            transition = false;
            t_frame = 0;
        }

        if (t_frame == FRAMES_IN) {
            screen = nxt_screen;
        }
    }
}

struct Button {
    Rectangle rect;
    Color fill_color, line_color;
    std::string name;

    Button(Rectangle r, Color fc, Color lc, std::string s) : rect(r), fill_color(fc), line_color(lc), name(s) {}

    void draw() {
        DrawRectangleRec(rect, fill_color);
        DrawRectangleLinesEx(rect, 1, line_color);
        DrawText(name.c_str(), rect.x + 10, rect.y + 10, 20, line_color);
        if (CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            DrawRectangleRec(rect, {0, 0, 0, 75});
        }
    }

    bool is_pressed() {
        return CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    }
};

struct NavButton : public Button {
    int dest;
    bool activated = false;
    int since_pressed = 0;

    NavButton(Rectangle r, Color fc, Color lc, std::string s, int d) : Button(r, fc, lc, s), dest(d) {}

    void refresh() {
        draw();
        activated |= is_pressed();

        if (activated) {
            if ((frame / 3) % 2) {
                DrawRectangleRec(rect, {255, 255, 255, 75});
            }
            if (since_pressed++ == 20) {
                activated = false;
                since_pressed = 0;

                nxt_screen = dest;
                transition = true;
            }
        }
    }
};

template <class F>
struct FuncButton : public Button {
    F func;

    FuncButton(Rectangle r, Color fc, Color lc, std::string s, F f) : Button(r, fc, lc, s), func(f) {}

    void refresh() {
        draw();
        if (is_pressed()) {
            func();
        }
    }
};

void draw_menu() {
    ClearBackground(RAYWHITE);
    DrawTexture(catjobs, 30, 30, WHITE);
    DrawText("CatJobs v0.2", 120, 35, 25, DARKGRAY);
    DrawText("By Mark Zhou", 120, 60, 15, LIGHTGRAY);
    DrawRectangle(25, 125, 400, 2, LIGHTGRAY);

    static NavButton simulate({30, 150, 175, 40}, GOLD, DARKORANGE, "Simulate", 1);
    simulate.refresh();

    static NavButton strategize({30, 200, 175, 40}, GOLD, DARKORANGE, "Strategize", 2);
    strategize.refresh();

    static NavButton benchmark({30, 250, 175, 40}, LIGHTGRAY, DARKGRAY, "Benchmark", 3);
    benchmark.refresh();

    static NavButton settings({30, 300, 175, 40}, LIGHTGRAY, DARKGRAY, "Settings", 4);
    settings.refresh();

    static NavButton about({30, 350, 175, 40}, LIGHTGRAY, DARKGRAY, "About", 5);
    about.refresh();
}

std::atomic<bool> bm_running(0);
std::atomic<int> bm_result(-1);
Timer bm_timer;

void run_benchmark() {
    bm_running = true;
    bm_result = benchmark_a();
    bm_running = false;
}

void draw_benchmark() {
    ClearBackground(RAYWHITE);

    DrawText("Benchmark", 30, 30, 25, DARKGRAY);
    DrawRectangle(25, 70, 400, 2, LIGHTGRAY);

    DrawText((bm_running ? "Running..." : bm_result != -1 ? std::to_string(bm_result) : "Press run to begin.").c_str(), 30, 90, 20, DARKGRAY);
    DrawText("Frames computed/second, average over a 5s sample", 30, 113, 15, LIGHTGRAY);

    Rectangle bar{30, 145, 500, 30};
    DrawRectangleRec(bar, LIGHTGRAY);
    DrawRectangleLinesEx(bar, 1, DARKGRAY);
    if (bm_running) {
        DrawRectangle(bar.x + 1, bar.y + 1, bm_timer.get() / 5000.0f * bar.width - 2, bar.height - 2, LIME);
    }

    static FuncButton run({30, 190, 140, 40}, LIGHTGRAY, DARKGRAY, "Run", [&]() {
        if (!bm_running) {
            std::thread(run_benchmark).detach();
            bm_timer.set();
        }
    });
    run.refresh();

    static NavButton back({30, WINDOW_H - 70, 100, 40}, LIGHTGRAY, DARKGRAY, "Back", 0);
    back.refresh();
}

void draw_about() {
    ClearBackground(RAYWHITE);

    DrawText("About", 30, 30, 25, DARKGRAY);
    DrawRectangle(25, 70, 400, 2, LIGHTGRAY);

    static NavButton back({30, WINDOW_H - 70, 100, 40}, LIGHTGRAY, DARKGRAY, "Back", 0);
    back.refresh();
}

void draw_filler() {
    ClearBackground(RAYWHITE);

    static NavButton back({30, WINDOW_H - 70, 100, 40}, LIGHTGRAY, DARKGRAY, "Back", 0);
    back.refresh();
}

int main() {
    InitWindow(WINDOW_W, WINDOW_H, "CatJobs v0.2");
    SetTargetFPS(60);

    catjobs = LoadTexture("catjobs_square.png");

    while (!WindowShouldClose()) {
        BeginDrawing();

        switch (screen) {
        case 0:
            draw_menu();
            break;
        case 3:
            draw_benchmark();
            break;
        case 5:
            draw_about();
            break;
        default:
            draw_filler();
            break;
        }

        handle_transition();

        DrawCircleV(GetMousePosition(), 5, MAROON);

        EndDrawing();
        frame++;
    }
    CloseWindow();
}