#pragma once

#include <unistd.h>

#include <cstdlib>

#include "defs.h"
#include "raylib.h"
#include "util.h"

enum class MapType {
  Nothing,
  Fixed,
  Levitating,
  Burn,
};

int map_type_to_int(MapType mapType) {
  switch (mapType) {
    case MapType::Nothing:
      return 0;
    case MapType::Fixed:
      return 1;
    case MapType::Levitating:
      return 2;
    case MapType::Burn:
      return 3;
    default:
      exit(EXIT_FAILURE);
  }
}

MapType map_type_from_int(int mapType) {
  switch (mapType) {
    case 0:
      return MapType::Nothing;
    case 1:
      return MapType::Fixed;
    case 2:
      return MapType::Levitating;
    case 3:
      return MapType::Burn;
    default:
      exit(EXIT_FAILURE);
  }
}

Color map_type_to_color(MapType mapType) {
  switch (mapType) {
    case MapType::Nothing:
      return RAYWHITE;
    case MapType::Fixed:
      return BROWN;
    case MapType::Levitating:
      return BLUE;
    case MapType::Burn:
      return RED;
    default:
      exit(EXIT_FAILURE);
  }
}

/*

Map cell:
_ _ _ _ _ _ _ _
              ^-   0 byte: on/off
        ^ ^ ^--- 1-3 byte: cell type
*/

struct Map {
  Map() = default;
  ~Map() = default;

  void update() {
    for (int y = PIXEL_HEIGHT - 1; y >= 1; y--) {
      for (int x = PIXEL_WIDTH - 1; x >= 0; x--) {
        if (is_pixel_on(x, y) && get_pixel_map_type(x, y) == MapType::Fixed) {
          if (!is_pixel_on(x, y + 1)) {
            set_pixel(x, y + 1, get_pixel(x, y));
            unset_pixel(x, y);
          } else {
            if (x > 0 && rand() % 3 == 0) {
              if (!is_pixel_on(x - 1, y + 1)) {
                set_pixel(x - 1, y + 1, get_pixel(x, y));
                unset_pixel(x, y);
              }
            } else if (x < WIDTH - 1 && rand() % 2 == 0) {
              if (!is_pixel_on(x + 1, y + 1)) {
                set_pixel(x + 1, y + 1, get_pixel(x, y));
                unset_pixel(x, y);
              }
            }
          }
        }
      }
    }
  }

  bool is_pixel_on(int x, int y) { return map[y][x] > 0; }

  void set_pixel(int x, int y) { set_pixel(x, y, MapType::Fixed); }

  void unset_pixel(int x, int y) {
    if (in_range(x, y, PIXEL_WIDTH, PIXEL_HEIGHT)) {
      map[y][x] = 0;
    }
  }

  uint8_t get_pixel(int x, int y) {
    if (!in_range(x, y, PIXEL_WIDTH, PIXEL_HEIGHT)) {
      return 0;
    }

    return map[y][x];
  }

  void set_pixel(int x, int y, MapType mapType) {
    set_pixel(x, y, 1 | ((map_type_to_int(mapType) & 0b111) << 1));
  }

  void set_pixel(int x, int y, uint8_t pixelData) {
    if (in_range(x, y, PIXEL_WIDTH, PIXEL_HEIGHT)) {
      map[y][x] = pixelData;
    }
  }

  MapType get_pixel_map_type(int x, int y) {
    int mapType = (map[y][x] >> 1) & 0b111;
    return map_type_from_int(mapType);
  }

  Color get_pixel_color(int x, int y) {
    int mapType = (map[y][x] >> 1) & 0b111;
    return map_type_to_color(map_type_from_int(mapType));
  }

 private:
  uint8_t map[PIXEL_HEIGHT][PIXEL_WIDTH];
};
