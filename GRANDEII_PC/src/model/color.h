//
// Created by giangvu on 3/13/25.
//

/**
 * @file color.h
 * @brief Defines the Color structure and functions for handling colors.
 */

#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>  // For uint8_t and uint32_t
#include "../arduino/communication.h"
#include "list_common.h"

#define COMPARE_THRESHHOLD 100.0/// Maximum treshold for acceptable comparaison

/**
 * @struct Color
 * @brief Represents an RGB color with an auto-generated hexadecimal ID.
 */
typedef struct {
    uint8_t r;    /**< Red component (0-255) */
    uint8_t g;    /**< Green component (0-255) */
    uint8_t b;    /**< Blue component (0-255) */
    unsigned char ir_code[IR_CODE_LENGTH]; /**<Unique IR code for color */
} Color;

/**
 * @struct HSL
 * @brief Represents a color in the HSL (Hue, Saturation, Lightness) color space.
 *
 * This structure stores a color using its hue, saturation, and lightness components.
 */
typedef struct {
    float h; // Hue [0, 360)
    float s; // Saturation [0, 1]
    float l; // Lightness [0, 1]
}HSL;


/**
 * @brief Transform an RGB (red green blue) into a Hue Saturation Value one
 * @param r red component
 * @param g green component
 * @param b blue component
 * @return The HSL struct created
 */
HSL rgbToHsl(int r, int g, int b);

/**
     * @brief Allocate a Color structure.
     * @return pointer to the successfully allocated Color
     */
Color* alloc_color();

/**
 * @brief set a Color structure
 *
 * @param r Red component (0-255).
 * @param g Green component (0-255).
 * @param b Blue component (0-255).
 * @return 0 if successful, -1 if error
 */
int set_color(Color * this_color, uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Prints the color information including RGB values and its hex ID.
 *
 * @param color The Color structure to display.
 * @return 0 if success, -1 if error
 */
int print_color_info(Color* color);

/**
 * @brief Compare to an exact color
 * @param a color A to be compared
 * @param b color B to be compared
 * @return 1 if a=b in terms of color, 0 else
 */
int compareColorExact(const void* a, const void* b);

/**
 * @brief Return the distance (similitude) of the two colors
 * @param a Color A to be compared
 * @param b Color B to be compared
 * @return The distance of the two color
 * @note The closer to 0 the distance is, the similar the colors are
 */
int compareColorDist(const void* a, const void* b);


/**
 * @brief Free the color struct
 * @param data color struct
 */
void free_color(void* data);


#endif //COLOR_H
