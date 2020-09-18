#pragma once

#include "globals.h"
#include "Math.h"
#include "TileData.h"

class TileUtils {
public:
    static sf::Color LightLevelToColor(uchar);

    struct LightData {
        LightData(sf::Vector2i p, char v, sf::Color color) : position(p), value(v), color(color) {}

        sf::Vector2i position;
        char value;
        sf::Color color;
    };
};