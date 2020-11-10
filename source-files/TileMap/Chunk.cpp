#include "TileMap/Chunk.h"

Chunk::Chunk(sf::Vector2i p) : position(Math::GridToWorld(Math::ChunkToGrid(p))) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            tiles[x][y].chunk = this;
        }
    }
}

void Chunk::SetTile(sf::Vector2i p, Tile::ID id) {
    tiles[p.x][p.y].id = id;
    tiles[p.x][p.y].state = TILE_EMPTY_MASK;

    uint32 rand_texture_coord = STATE_COUNT * TILE_TEXTURE_SIZE;
    rand_texture_coord *= Math::Random(0, RAND_TILE_TEXTURE_COUNT);

    tiles[p.x][p.y].rand_texture_coord = rand_texture_coord;
}

Tile* Chunk::GetTile(sf::Vector2i p) {
    return &tiles[p.x][p.y];
}

void Chunk::UpdateMesh() {
    mesh.clear();

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            if (!GetTileData(tiles[x][y].id).has_mesh) continue;

            GenerateMeshForTile(sf::Vector2i(x, y), mesh.size());
        }
    }
}

const std::vector <sf::Vertex>& Chunk::GetMesh() {
    return mesh;
}

void Chunk::UpdateLight() {
    int vert_ind = 0;

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            if (!GetTileData(tiles[x][y].id).has_mesh) continue;

            for (int i = 0; i < 4; i++) {
                mesh[vert_ind + i].color = TileUtils::LightLevelToColor(tiles[x][y].light_level[i]);
            }

            vert_ind += 4;
        }
    }
}

/*
 * PRO : EXPECTED TO BE MUCH FASTER
 * CON : WILL NOT UPDATE TILE STATE
 *
 * void Chunk::UpdateMesh(sf::Vector2i tile_p, Tile::ID old_id) const {
    int new_id = tiles[tile_p.x][tile_p.y].id;

    bool old_has_mesh = GetTileData(old_id).has_mesh;
    bool new_has_mesh = GetTileData(new_id).has_mesh;

    if (old_has_mesh != new_has_mesh) {
        int vert_ind = 0;

        for (int x = 0; x <= tile_p.x; x++) {
            for (int y = 0; x == tile_p.x ? y < tile_p.y : y < CHUNK_SIZE; y++) {
                if (!GetTileData(tiles[x][y].id).has_mesh) continue;

                vert_ind += 4;
            }
        }

        if (new_has_mesh) {
            GenerateMeshForTile(tile_p, vert_ind);
        } else {
            mesh.erase(mesh.begin() + vert_ind + 1, mesh.begin() + vert_ind + 5);
        }
    }
}*/

void Chunk::GenerateMeshForTile(sf::Vector2i tile_p, int vert_ind) {
    uint texture_coord = tiles[tile_p.x][tile_p.y].rand_texture_coord + TILE_TEXTURE_SIZE * tiles[tile_p.x][tile_p.y].state;

    mesh.insert(mesh.begin() + vert_ind, sf::Vertex(
        (sf::Vector2f)position + sf::Vector2f(tile_p.x, tile_p.y) * GRID_SIZE,
        sf::Vector2f(texture_coord, tiles[tile_p.x][tile_p.y].id * TILE_TEXTURE_SIZE)
    ));

    mesh.insert(mesh.begin() + vert_ind, sf::Vertex(
            (sf::Vector2f)position + sf::Vector2f(tile_p.x + 1, tile_p.y) * GRID_SIZE,
            sf::Vector2f(texture_coord + TILE_TEXTURE_SIZE, tiles[tile_p.x][tile_p.y].id * TILE_TEXTURE_SIZE)
    ));

    mesh.insert(mesh.begin() + vert_ind, sf::Vertex(
            (sf::Vector2f)position + sf::Vector2f(tile_p.x + 1, tile_p.y + 1) * GRID_SIZE,
            sf::Vector2f(texture_coord + TILE_TEXTURE_SIZE, tiles[tile_p.x][tile_p.y].id * TILE_TEXTURE_SIZE + TILE_TEXTURE_SIZE)
    ));

    mesh.insert(mesh.begin() + vert_ind, sf::Vertex(
            (sf::Vector2f)position + sf::Vector2f(tile_p.x, tile_p.y + 1) * GRID_SIZE,
            sf::Vector2f(texture_coord, tiles[tile_p.x][tile_p.y].id * TILE_TEXTURE_SIZE + TILE_TEXTURE_SIZE)
    ));
}