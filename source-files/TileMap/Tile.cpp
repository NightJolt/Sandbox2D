#include "TileMap/Tile.h"

Tile::Tile() : id(Air), state(0), rand_texture_coord(0), chunk(nullptr) {
    memset(light_level, 0, 4);
}

void Tile::AppendState(uint8 state_mask) {
    if (state_mask <= TILE_FULL_MASK) state |= state_mask;
    else state &= state_mask;
}