#pragma once

#include "globals.h"
#include "Math.h"
#include "TileData.h"

class TileUtils {
public:
    static sf::Color LightLevelToColor(uint8);

    struct LightData {
        LightData(sf::Vector2i p, int8 v, sf::Color color) : position(p), value(v), color(color) {}

        sf::Vector2i position;
        int8 value;
        sf::Color color;
    };
};