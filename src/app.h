#pragma once

#include <cstdlib>
#include <iostream>
#include <string>

#include "defs.h"
#include "map.h"
#include "raylib.h"
#include "util.h"

using namespace std;

enum class SprayType {
  Rectangle,
  Scattered,
};

struct App {
  App() { init(); }

  ~App() = default;

  void loop() {
    while (!WindowShouldClose()) {
      update();

      BeginDrawing();

      ClearBackground(RAYWHITE);
      DrawFPS(4, 4);

      draw();

      EndDrawing();
    }
  }

 private:
  Map map{};
  int sprayRadius{PIXEL_SPRAY_MULTIPLIER + 1};
  MapType sprayMapType{MapType::Fixed};
  SprayType sprayType{SprayType::Rectangle};

  void init() {
    // SetConfigFlags(FLAG_WINDOW_HIGHDPI);

    InitWindow(WIDTH, HEIGHT, "Dotz");

    SetTargetFPS(120);
  }

  void update() {
    int keyPressed = GetKeyPressed();
    if (KEY_ZERO <= keyPressed && keyPressed <= KEY_NINE) {
      sprayRadius = (keyPressed - KEY_ZERO) * PIXEL_SPRAY_MULTIPLIER + 1;
    }

    if (IsKeyPressed(KEY_F)) sprayMapType = MapType::Fixed;
    if (IsKeyPressed(KEY_B)) sprayMapType = MapType::Burn;
    if (IsKeyPressed(KEY_L)) sprayMapType = MapType::Levitating;

    if (IsKeyPressed(KEY_S)) sprayType = SprayType::Scattered;
    if (IsKeyPressed(KEY_R)) sprayType = SprayType::Rectangle;

    if (IsMouseButtonDown(0)) {
      Vector2 mousePos = GetMousePosition();

      if (sprayType == SprayType::Scattered) {
        for (int i = 0; i < sprayRadius * sprayRadius; i++) {
          map.set_pixel(
              (mousePos.x + (rand() % (sprayRadius * 2)) - sprayRadius) /
                  PIXEL_SIZE,
              (mousePos.y + (rand() % (sprayRadius * 2)) - sprayRadius) /
                  PIXEL_SIZE,
              sprayMapType);
        }
      } else if (sprayType == SprayType::Rectangle) {
        for (int i = -sprayRadius; i <= sprayRadius; i++) {
          for (int j = -sprayRadius; j <= sprayRadius; j++) {
            map.set_pixel(mousePos.x / PIXEL_SIZE + i,
                          mousePos.y / PIXEL_SIZE + j, sprayMapType);
          }
        }
      }
    }

    if (IsMouseButtonDown(1)) {
      Vector2 mousePos = GetMousePosition();
      for (int i = -(PIXEL_ERASER_SIZE / 2); i <= (PIXEL_ERASER_SIZE / 2);
           i++) {
        for (int j = -(PIXEL_ERASER_SIZE / 2); j <= (PIXEL_ERASER_SIZE / 2);
             j++) {
          map.unset_pixel(mousePos.x / PIXEL_SIZE + i,
                          mousePos.y / PIXEL_SIZE + j);
        }
      }
    }

    map.update();
  }

  void draw() {
    for (int y = 0; y < PIXEL_HEIGHT; y++) {
      for (int x = 0; x < PIXEL_WIDTH; x++) {
        if (map.is_pixel_on(x, y)) {
          DrawRectangle(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE,
                        map.get_pixel_color(x, y));
        }
      }
    }
  }
};
