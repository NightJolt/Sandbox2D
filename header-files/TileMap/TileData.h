#pragma once

#include "globals.h"

#define MAX_LIGHT_LEVEL (ushort)16

struct TileData {
    ushort light_emission = 0;
};

const TileData TILE_DATA[] = {
        { // AIR
            0
        },
        { // DIRT
            0
        },
        { // COBBLE
            0
        },
        { // SAND
            MAX_LIGHT_LEVEL
        }
};

inline const TileData& GetTileData(int id) {
    return TILE_DATA[id + 1]; // +1 cuz AIR is -1
}