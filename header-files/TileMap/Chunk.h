#pragma once

#include <SFML/Graphics.hpp>

#include "Tile.h"
#include "Math.h"
#include "TileData.h"

class Chunk {
public:
    explicit Chunk(sf::Vector2i);

    void SetTile(sf::Vector2i, Tile::ID, uchar);
    const Tile* GetTile(sf::Vector2i);

    void GenerateMesh();
    void UpdateMesh(sf::Vector2i);
    //void UpdateLight(sf::Vector2i);

    [[nodiscard]] const std::vector <sf::Vertex>& GetMesh() const;

private:
    sf::Vector2i position;

    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];

    std::vector <sf::Vertex> mesh;
};