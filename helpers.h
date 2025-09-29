#ifndef HELPERS_H
#define HELPERS_H

/**
 * Helper methods for the Box2D implementation within this project.
 * Assumes that Box2D worlds are created by converting pixel coordinates to world
 * coordinates using these methods, and that a world's origin is the bottom left of screen.
 * @author Danny Provo
 * @date 4/7/2025
 */
namespace Box2DHelpers {

    const int PIXELS_PER_METER = 10;

    /**
     * Converts the world x coordinate to a Pixel x coordinate
     * @param worldX the world x coordinate as a floating point number
     * @return an int representing the Pixel x coordinate
     */
    int worldXToPixelX(float worldX);

    /**
     * Converts the world y coordinate to a Pixel y coordinate
     * @param worldY the world y coordinate as a floating point number
     * @param wigetHeight the height of the widget containing the animation (in pixels)
     * @return an int representing the Pixel y coordinate
     */
    int worldYToPixelY(float worldY, int widgetHeight);

    /**
     * Converts a pixel x coordinate to a world x coordinate
     * @param pixelX the pixel x coordinate
     * @return the respective world x coordinate
     */
    float pixelXToWorldX(int pixelX);

    /**
     * Converts a pixel y coordinate to a world y coordinate
     * @param pixelY the pixel y coordinate
     * @param wigetHeight the height of the widget containing the animation (in pixels)
     * @return the respective world y coordinate
     */
    float pixelYToWorldY(int pixelY, int widgetHeight);
}

#endif // HELPERS_H
