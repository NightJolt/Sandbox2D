#include "TileMap/TileMap.h"

TileMap::TileMap() = default;

TileMap::~TileMap() noexcept {
    for (auto& [k, v] : chunks) delete v;
}

void TileMap::draw(sf::RenderTarget& rt, sf::RenderStates rs) const {
    const sf::View view = rt.getView();

    const sf::Vector2f view_center = view.getCenter();
    const sf::Vector2f view_size = view.getSize();

    const sf::Vector2f camera_start_p = view_center - view_size * .5f;
    const sf::Vector2f camera_end_p = view_center + view_size * .5f;

    const sf::Vector2i s = Math::GridToChunk(Math::WorldToGrid(camera_start_p));
    const sf::Vector2i e = Math::GridToChunk(Math::WorldToGrid(camera_end_p));

    for (int x = s.x; x <= e.x; x++) {
        for (int y = s.y; y <= e.y; y++) {
            const std::string key = to_string_unformatted(sf::Vector2i(x, y));
            if (!chunks.contains(key)) continue;

            const Chunk* chunk = chunks[key];

            rt.draw(&chunk->GetMesh()[0], chunk->GetMesh().size(), sf::Quads, sf::RenderStates(&R::textures[0]));
        }
    }
}


/* ===== CHUNK ===== */

Chunk* TileMap::CreateChunk(sf::Vector2i p) {
    const std::string key = to_string_unformatted(p);

    if (!chunks.contains(key)) chunks[key] = new Chunk(p);

    return chunks[key];
}

Chunk* TileMap::GetChunk(sf::Vector2i p) {
    const std::string key = to_string_unformatted(p);

    if (!chunks.contains(key)) return nullptr;

    return chunks[key];
}

void TileMap::RefreshChunk(sf::Vector2i chunk_p) {
    Chunk* chunk = GetChunk(chunk_p);

    if (chunk) chunk->GenerateMesh();
}

/* ================= */


/* ===== TILE ===== */

void TileMap::SetTile(sf::Vector2i grid_p, Tile::ID id, bool allow_overwrite, bool update_chunk) {
    sf::Vector2i chunk_position = Math::GridToChunk(grid_p);

    Chunk* chunk = CreateChunk(chunk_position);

    int tile_id = GetTileID(grid_p);

    if (allow_overwrite || tile_id == Tile::Air) {
        chunk->SetTile(Math::GridToTile(grid_p), id, TILE_EMPTY_MASK);

        if (id != Tile::Air || tile_id != Tile::Air) UpdateTileState(grid_p);

        const TileData td_o = GetTileData(tile_id);
        const TileData td_n = GetTileData(id);

        short light_level = td_n.light_emission - td_o.light_emission;

        if (light_level) light_queue.push(LightData(grid_p, light_level));

        if (update_chunk) UpdateLight(), chunk->GenerateMesh();
    }
}

const Tile* TileMap::GetTile(sf::Vector2i p) {
    sf::Vector2i chunk_position = Math::GridToChunk(p);
    const std::string key = to_string_unformatted(chunk_position);

    if (!chunks.contains(key)) return nullptr;

    return chunks[key]->GetTile(Math::GridToTile(p));
}

Tile::ID TileMap::GetTileID(sf::Vector2i p) {
    const Tile* tile = GetTile(p);

    return tile ? tile->id : Tile::Air;
}

void TileMap::UpdateTileState(sf::Vector2i p) {
    uchar mask = 0;
    const Tile* tile;

    const Tile* self_tile = GetTile(p);
    int self_id = self_tile->id;

    sf::Vector2i self_chunk_p = Math::GridToChunk(p);
    sf::Vector2i chunk_p;

    // RIGHT
    tile = GetTile(p + sf::Vector2i(-1, 0));
    if (tile && tile->id != Tile::Air) {
        mask |= TILE_LEFT_MASK;

        if (self_id == Tile::Air)
            tile->AppendState(~TILE_RIGHT_MASK);
        else
            tile->AppendState(TILE_RIGHT_MASK);

    }

    chunk_p = Math::GridToChunk(p + sf::Vector2i(-1, 0));
    if (chunk_p != self_chunk_p) RefreshChunk(chunk_p);

    // LEFT
    tile = GetTile(p + sf::Vector2i(1, 0));
    if (tile && tile->id != Tile::Air) {
        mask |= TILE_RIGHT_MASK;

        if (self_id == Tile::Air)
            tile->AppendState(~TILE_LEFT_MASK);
        else
            tile->AppendState(TILE_LEFT_MASK);

    }

    chunk_p = Math::GridToChunk(p + sf::Vector2i(1, 0));
    if (chunk_p != self_chunk_p) RefreshChunk(chunk_p);

    // UP
    tile = GetTile(p + sf::Vector2i(0, -1));
    if (tile && tile->id != Tile::Air) {
        mask |= TILE_UP_MASK;

        if (self_id == Tile::Air)
            tile->AppendState(~TILE_DOWN_MASK);
        else
            tile->AppendState(TILE_DOWN_MASK);

    }

    chunk_p = Math::GridToChunk(p + sf::Vector2i(0, -1));
    if (chunk_p != self_chunk_p) RefreshChunk(chunk_p);

    // DOWN
    tile = GetTile(p + sf::Vector2i(0, 1));
    if (tile && tile->id != Tile::Air) {
        mask |= TILE_DOWN_MASK;

        if (self_id == Tile::Air)
            tile->AppendState(~TILE_UP_MASK);
        else
            tile->AppendState(TILE_UP_MASK);

    }

    chunk_p = Math::GridToChunk(p + sf::Vector2i(0, 1));
    if (chunk_p != self_chunk_p) RefreshChunk(chunk_p);

    self_tile->AppendState(mask);
}

/* ================ */


void TileMap::UpdateLight() {
    std::unordered_map <std::string, bool> fix;

    while (!light_queue.empty()) {
        LightData ld = light_queue.front();
        light_queue.pop();

        const Tile* tile = GetTile(ld.p);

        if (tile) {
            for (auto& light : tile->light_level) {
                light = ld.v;
            }
        }
    }
}