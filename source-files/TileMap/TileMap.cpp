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

    UpdateLightLevels();

    for (int x = s.x; x <= e.x; x++) {
        for (int y = s.y; y <= e.y; y++) {
            const Chunk* chunk = GetChunk(sf::Vector2i(x, y));

            if (!chunk) continue;

            chunk->UpdateMesh();
            chunk->UpdateLight();

            rt.draw(&chunk->GetMesh()[0], chunk->GetMesh().size(), sf::Quads, sf::RenderStates(&R::textures[0]));
        }
    }
}


/* ===== CHUNK ===== */

Chunk* TileMap::CreateChunk(sf::Vector2i chunk_p) {
    const std::string key = to_string_unformatted(chunk_p);

    if (!chunks.contains(key)) chunks[key] = new Chunk(chunk_p);

    return chunks[key];
}

Chunk* TileMap::GetChunk(sf::Vector2i chunk_p) const {
    const std::string key = to_string_unformatted(chunk_p);

    if (!chunks.contains(key)) return nullptr;

    return chunks[key];
}

/* ================= */


/* ===== TILE ===== */

void TileMap::SetTile(sf::Vector2i grid_p, Tile::ID id, bool allow_overwrite) {
    sf::Vector2i chunk_position = Math::GridToChunk(grid_p);

    Chunk* chunk = CreateChunk(chunk_position);

    int old_tile_id = GetTileID(grid_p);

    if (allow_overwrite || !GetTileData(old_tile_id).has_mesh) {
        chunk->SetTile(Math::GridToTile(grid_p), id);
        UpdateTileState(grid_p);
    }
}

const Tile* TileMap::GetTile(sf::Vector2i grid_p) const {
    sf::Vector2i chunk_position = Math::GridToChunk(grid_p);
    const std::string key = to_string_unformatted(chunk_position);

    if (!chunks.contains(key)) return nullptr;

    return chunks[key]->GetTile(Math::GridToTile(grid_p));
}

Tile::ID TileMap::GetTileID(sf::Vector2i p) const {
    const Tile* tile = GetTile(p);

    return tile ? tile->id : Tile::Air;
}

void TileMap::UpdateTileState(sf::Vector2i grid_p) const {
    uchar mask = 0;
    const Tile* other_tile;
    const Tile* self_tile = GetTile(grid_p);

    // RIGHT
    other_tile = GetTile(grid_p + sf::Vector2i(1, 0));
    if (other_tile && GetTileData(other_tile->id).has_mesh) {
        mask |= TILE_RIGHT_MASK;

        if (GetTileData(self_tile->id).has_mesh)
            other_tile->AppendState(TILE_LEFT_MASK);
        else
            other_tile->AppendState(~TILE_LEFT_MASK);

        ((Chunk*)other_tile->chunk)->must_update_mesh = true;
    }

    // LEFT
    other_tile = GetTile(grid_p + sf::Vector2i(-1, 0));
    if (other_tile && GetTileData(other_tile->id).has_mesh) {
        mask |= TILE_LEFT_MASK;

        if (GetTileData(self_tile->id).has_mesh)
            other_tile->AppendState(TILE_RIGHT_MASK);
        else
            other_tile->AppendState(~TILE_RIGHT_MASK);

        ((Chunk*)other_tile->chunk)->must_update_mesh = true;
    }

    // UP
    other_tile = GetTile(grid_p + sf::Vector2i(0, -1));
    if (other_tile && GetTileData(other_tile->id).has_mesh) {
        mask |= TILE_UP_MASK;

        if (GetTileData(self_tile->id).has_mesh)
            other_tile->AppendState(TILE_DOWN_MASK);
        else
            other_tile->AppendState(~TILE_DOWN_MASK);

        ((Chunk*)other_tile->chunk)->must_update_mesh = true;
    }

    // DOWN
    other_tile = GetTile(grid_p + sf::Vector2i(0, 1));
    if (other_tile && GetTileData(other_tile->id).has_mesh) {
        mask |= TILE_DOWN_MASK;

        if (GetTileData(self_tile->id).has_mesh)
            other_tile->AppendState(TILE_UP_MASK);
        else
            other_tile->AppendState(~TILE_UP_MASK);

        ((Chunk*)other_tile->chunk)->must_update_mesh = true;
    }

    self_tile->AppendState(mask);
}

/* ================ */


void TileMap::UpdateLightLevels() const {
    /*std::unordered_set <std::string> fix;

    auto push_light = [&](char v) {
        if (v) {
            light_queue.push(LightData(grid_p, v));
            light_queue.push(LightData(grid_p + sf::Vector2(0, 1), v));
            light_queue.push(LightData(grid_p + sf::Vector2(1, 1), v));
            light_queue.push(LightData(grid_p + sf::Vector2(1, 0), v));
        }
    };

    auto spread_light = [&](sf::Vector2i p, char v) {
        if (v > 0) {
            light_queue.push(LightData(p + sf::Vector2i(0, -1), v, false));
            light_queue.push(LightData(p + sf::Vector2i(0, 1), v, false));
            light_queue.push(LightData(p + sf::Vector2i(-1, 0), v, false));
            light_queue.push(LightData(p + sf::Vector2i(1, 0), v, false));
        }
    };

    while (!light_queue.empty()) {
        LightData ld = light_queue.front();
        light_queue.pop();

        std::string key = to_string_unformatted(ld.position);
        if (!fix.contains(key)) continue;

        fix.insert(key);

        const Tile* tile;
        Chunk* chunk;
        bool has_mesh;
        char new_light;

        // DOWN_RIGH

        tile = GetTile(ld.position);

        if (tile) {
            chunk = (Chunk*)tile->chunk;
            has_mesh = GetTileData(tile->id).has_mesh;
            new_light = has_mesh ? new_tile_light : new_space_light;

            tile->light_level[0] = new_light;
            chunk->must_update_light = true;

            //push_light(ld.position, new_light);
        }

        // UP_LEFT

        tile = GetTile(ld.position + sf::Vector2(-1, -1));

        if (tile) {
            chunk = (Chunk*)tile->chunk;
            has_mesh = GetTileData(tile->id).has_mesh;
            new_light = has_mesh ? new_tile_light : new_space_light;

            tile->light_level[3] = new_light;
            chunk->must_update_light = true;

            //push_light(ld.position, new_light);
        }

        // DOWN_LEFT

        tile = GetTile(ld.position + sf::Vector2(-1, 0));

        if (tile) {
            chunk = (Chunk*)tile->chunk;
            has_mesh = GetTileData(tile->id).has_mesh;
            new_light = has_mesh ? new_tile_light : new_space_light;

            tile->light_level[2] = new_light;
            chunk->must_update_light = true;

            //push_light(ld.position, new_light);
        }

        // UP_RIGH

        tile = GetTile(ld.position + sf::Vector2(0, -1));

        if (tile) {
            chunk = (Chunk*)tile->chunk;
            has_mesh = GetTileData(tile->id).has_mesh;
            new_light = has_mesh ? new_tile_light : new_space_light;

            tile->light_level[1] = new_light;
            chunk->must_update_light = true;

            //push_light(ld.position, new_light);
        }
    }*/
}