#include "TileMap/Chunk.h"

Chunk::Chunk(sf::Vector2i p) : position(Math::GridToWorld(Math::ChunkToGrid(p))) {}

void Chunk::SetTile(sf::Vector2i p, Tile::ID id, uchar state_mask) {
    tiles[p.x][p.y].id = id;
    tiles[p.x][p.y].state = state_mask;
    tiles[p.x][p.y].rand_texture_pos = Math::Random(0, RAND_TILE_TEXTURE_COUNT);
}

const Tile* Chunk::GetTile(sf::Vector2i p) {
    return &tiles[p.x][p.y];
}

void Chunk::GenerateMesh() {
    mesh.clear();

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE ;y++) {
            if (tiles[x][y].id < 0) continue;

            uint texture_coord = STATE_COUNT * TILE_TEXTURE_SIZE;
            texture_coord *= tiles[x][y].rand_texture_pos;
            texture_coord += TILE_TEXTURE_SIZE * tiles[x][y].state;

            int id = tiles[x][y].id;

            sf::Vertex vertex;
            short light_color;

            vertex.position = (sf::Vector2f)position + sf::Vector2f(x, y) * GRID_SIZE;
            vertex.texCoords = sf::Vector2f(texture_coord, id * TILE_TEXTURE_SIZE);
            light_color = (short)Math::MapValueClamped(tiles[x][y].light_level[0], 0, MAX_LIGHT_LEVEL, 0, 255);
            vertex.color = sf::Color(light_color, light_color, light_color, 255);
            mesh.push_back(vertex);

            if (tiles[x][y].light_level[0]) std::cout << tiles[x][y].light_level[0] << " " << vertex.color << std::endl;

            vertex.position = (sf::Vector2f)position + sf::Vector2f(x + 1, y) * GRID_SIZE;
            vertex.texCoords = sf::Vector2f(texture_coord + TILE_TEXTURE_SIZE, id * TILE_TEXTURE_SIZE);
            light_color = (short)Math::MapValueClamped(tiles[x][y].light_level[0], 0, MAX_LIGHT_LEVEL, 0, 255);
            vertex.color = sf::Color(light_color, light_color, light_color, 255);
            mesh.push_back(vertex);

            vertex.position = (sf::Vector2f)position + sf::Vector2f(x + 1, y + 1) * GRID_SIZE;
            vertex.texCoords = sf::Vector2f(texture_coord + TILE_TEXTURE_SIZE, id * TILE_TEXTURE_SIZE + TILE_TEXTURE_SIZE);
            light_color = (short)Math::MapValueClamped(tiles[x][y].light_level[0], 0, MAX_LIGHT_LEVEL, 0, 255);
            vertex.color = sf::Color(light_color, light_color, light_color, 255);
            mesh.push_back(vertex);

            vertex.position = (sf::Vector2f)position + sf::Vector2f(x, y + 1) * GRID_SIZE;
            vertex.texCoords = sf::Vector2f(texture_coord, id * TILE_TEXTURE_SIZE + TILE_TEXTURE_SIZE);
            light_color = (short)Math::MapValueClamped(tiles[x][y].light_level[0], 0, MAX_LIGHT_LEVEL, 0, 255);
            vertex.color = sf::Color(light_color, light_color, light_color, 255);
            mesh.push_back(vertex);
        }
    }
}

const std::vector<sf::Vertex> &Chunk::GetMesh() const {
    return mesh;
}