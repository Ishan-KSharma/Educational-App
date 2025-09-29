/**
 * Helper methods for the Box2D implementation within this project.
 * Assumes that Box2D worlds are created by converting pixel coordinates to world
 * coordinates using these methods, and that a world's origin is the bottom left of screen.
 * @author Danny Provo
 * @date 4/7/2025
 */

#include "helpers.h"
#include <cmath>

using Box2DHelpers::PIXELS_PER_METER;
using std::round;

int Box2DHelpers::worldXToPixelX(float worldX) {
    return round(worldX * PIXELS_PER_METER);
}

int Box2DHelpers::worldYToPixelY(float worldY, int widgetHeight) {
    int maxPixelYCord = widgetHeight - 1;
    return maxPixelYCord - round(worldY * PIXELS_PER_METER);
}

float Box2DHelpers::pixelXToWorldX(int pixelX) {
    return pixelX / (float) PIXELS_PER_METER;
}

float Box2DHelpers::pixelYToWorldY(int pixelY, int widgetHeight) {
    int maxPixelYCord = widgetHeight - 1;
    return (maxPixelYCord - pixelY) / (float) PIXELS_PER_METER;
}
