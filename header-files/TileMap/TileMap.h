#pragma once

#include "Chunk.h"
#include "R.h"

#include <queue>

class TileMap : public sf::Drawable {
public:
    TileMap();
    ~TileMap() noexcept override;

    Chunk* GetChunk(sf::Vector2i);

    void SetTile(sf::Vector2i, Tile::ID, bool = false, bool = true);
    const Tile* GetTile(sf::Vector2i);
    Tile::ID GetTileID(sf::Vector2i);

private:
    Chunk* CreateChunk(sf::Vector2i);
    void UpdateTileState(sf::Vector2i);
    void RefreshChunk(sf::Vector2i);

    void draw(sf::RenderTarget&, sf::RenderStates) const override;

    mutable std::unordered_map <std::string, Chunk*> chunks;

    struct LightData {
        LightData(sf::Vector2i p, ushort v) : p(p), v(v) {}

        sf::Vector2i p;
        short v;

        static const ushort SPACE_SPREAD = 1;
        static const ushort TILE_SPREAD = 4;
    };

    std::queue <LightData> light_queue;

    void UpdateLight();
};