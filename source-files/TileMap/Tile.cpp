#include "TileMap/Tile.h"

Tile::Tile() : id(Air), state(0), rand_texture_pos(0) {
    for (auto& i : light_level) {
        i = 0;
    }
}

void Tile::AppendState(uchar state_mask) const {
    if (state_mask <= TILE_FULL_MASK) state |= state_mask;
    else state &= state_mask;
}