#pragma once

#include "globals.h"

#define TILE_UP_MASK      (uint8)1 << 0
#define TILE_RIGHT_MASK   (uint8)1 << 1
#define TILE_DOWN_MASK    (uint8)1 << 2
#define TILE_LEFT_MASK    (uint8)1 << 3
#define TILE_EMPTY_MASK   (uint8)0
#define TILE_FULL_MASK    (TILE_UP_MASK | TILE_RIGHT_MASK | TILE_DOWN_MASK | TILE_LEFT_MASK)

#define STATE_COUNT 16
#define TILE_TEXTURE_SIZE 8
#define RAND_TILE_TEXTURE_COUNT 2

struct Tile {
    enum ID {
        Air = -1,
        Dirt,
        Cobble,
        Sand,

        Count
    };

    Tile();

    void AppendState(uint8);

    ID id;
    uint8 state;
    uint8 rand_texture_coord;
    uint8 light_level[4];

    void* chunk;
};