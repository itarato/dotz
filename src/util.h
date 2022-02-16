#pragma once

#include "raylib.h"

bool in_range(int x, int y, int width, int height) {
  return x >= 0 && y >= 0 && x < width && y < height;
}

// bool in_range(Vector2 point, int width, int height) {
//   return point.x >= 0 && point.y >= 0 && point.x < width && point.y < height;
// }
