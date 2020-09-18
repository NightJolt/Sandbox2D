#pragma once

#include <SFML/Graphics.hpp>

#include "Tile.h"
#include "Math.h"
#include "TileData.h"
#include "TileUtils.h"

class Chunk {
public:
    explicit Chunk(sf::Vector2i);

    void SetTile(sf::Vector2i, Tile::ID);
    const Tile* GetTile(sf::Vector2i);

    void UpdateMesh() const;
    //void UpdateMesh(sf::Vector2i, Tile::ID) const;
    void UpdateLight() const;

    [[nodiscard]] const std::vector <sf::Vertex>& GetMesh() const;

    mutable bool must_update_mesh;
    mutable bool must_update_light;

private:
    void GenerateMeshForTile(sf::Vector2i, int) const;

    sf::Vector2i position;

    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
    mutable std::vector <sf::Vertex> mesh;

    std::vector <TileUtils::LightData> light_sources;
};