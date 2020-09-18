#pragma once

#include "Chunk.h"
#include "R.h"

#include <queue>
#include <unordered_set>

class TileMap : public sf::Drawable {
public:
    TileMap();
    ~TileMap() noexcept override;

    Chunk* GetChunk(sf::Vector2i) const;

    void SetTile(sf::Vector2i, Tile::ID, bool = false);
    const Tile* GetTile(sf::Vector2i) const;
    Tile::ID GetTileID(sf::Vector2i) const;

private:
    Chunk* CreateChunk(sf::Vector2i);
    void UpdateTileState(sf::Vector2i) const;

    void draw(sf::RenderTarget&, sf::RenderStates) const override;

    mutable std::unordered_map <std::string, Chunk*> chunks;

    mutable std::queue <TileUtils::LightData> light_queue;

    void UpdateLightLevels() const;
};