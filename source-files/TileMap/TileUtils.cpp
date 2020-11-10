#include "TileMap/TileUtils.h"

sf::Color TileUtils::LightLevelToColor(uint8 level) {
    uint8 col = Math::MapValueClamped(level, 0, MAX_LIGHT_LEVEL, 0, 255);

    return sf::Color(col, col, col, 255);
}