#pragma once

#include "globals.h"

#define TILE_UP_MASK (uchar)1
#define TILE_RIGHT_MASK (uchar)2
#define TILE_DOWN_MASK (uchar)4
#define TILE_LEFT_MASK (uchar)8

#define TILE_EMPTY_MASK (uchar)0
#define TILE_FULL_MASK (TILE_UP_MASK | TILE_RIGHT_MASK | TILE_DOWN_MASK | TILE_LEFT_MASK)

#define STATE_COUNT 16
#define TILE_TEXTURE_SIZE 8
#define RAND_TILE_TEXTURE_COUNT 2

struct Tile {
    enum ID {
        Air = -1,
        Dirt,
        Cobble,
        Sand,

        Size
    };

    Tile();

    void AppendState(uchar) const;

    ID id;
    mutable uchar state;
    uchar rand_texture_pos;
    mutable ushort light_level[4];
};