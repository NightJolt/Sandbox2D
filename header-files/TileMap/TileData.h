#pragma once

#include "globals.h"

#define MAX_LIGHT_LEVEL (uint8)16

struct TileData {
    std::string name;

    bool has_mesh = false;

    int8 light_emission = 0;
    int8 light_absorption = 0;
};

const TileData TILE_DATA[] = {
        {
            "air",
            false,
            0,
            1
        },
        {
            "dirt",
            true,
            6,
            4
        },
        {
            "cobble",
            true,
            0,
            4
        },
        {
            "sand",
            true,
            MAX_LIGHT_LEVEL,
            4
        }
};

inline const TileData& GetTileData(int id) {
    return TILE_DATA[id + 1]; // +1 cuz AIR id is -1
}