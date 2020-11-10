#pragma once

#include "Chunk.h"
#include "R.h"

//#include <queue>
#include <unordered_set>

class TileMap {
public:
    TileMap();
    ~TileMap();

    Chunk* GetChunk(sf::Vector2i);

    void SetTile(sf::Vector2i, Tile::ID, bool = false);
    Tile* GetTile(sf::Vector2i);
    Tile::ID GetTileID(sf::Vector2i);

    void Draw(sf::RenderTarget&);

private:
    Chunk* CreateChunk(sf::Vector2i);
    void UpdateTileState(sf::Vector2i);

    std::unordered_map <std::string, Chunk*> chunks;
    //std::queue <TileUtils::LightData> light_queue;

    //void UpdateLightLevels();
};