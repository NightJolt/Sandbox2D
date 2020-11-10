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
    Tile* GetTile(sf::Vector2i);

    void UpdateMesh();
    //void UpdateMesh(sf::Vector2i, Tile::ID);
    void UpdateLight();

    const std::vector <sf::Vertex>& GetMesh();

private:
    void GenerateMeshForTile(sf::Vector2i, int);

    sf::Vector2i position;

    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
    std::vector <sf::Vertex> mesh;

    //std::vector <TileUtils::LightData> light_sources;
};