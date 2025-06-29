#include <stdio.h>
#include "color.h"
#include <stdlib.h>
#include <math.h>


Color* alloc_color() {
    Color* this_color = malloc(sizeof(Color));

    if (this_color == NULL) {
        fprintf(stderr, "Memory allocation failed for color\n");
        return NULL;  // Or handle it according to your needs
    }
    return this_color;
}

int set_color(Color* this_color, uint8_t r, uint8_t g, uint8_t b) {

    if (this_color == NULL) {
        fprintf(stderr, "Color not yet allocated\n");
        return -1;
    }
    // Clamp values within 0-255 range
    this_color->r = (r > 255) ? 255 : (r < 0 ? 0 : r);
    this_color->g = (g > 255) ? 255 : (g < 0 ? 0 : g);
    this_color->b = (b > 255) ? 255 : (b < 0 ? 0 : b);

    return 0;
}

int print_color_info(Color* color){
    if (color != NULL) {
        printf("Color: R=%d, G=%d, B=%d\n", color->r, color->g, color->b);
        return 0;
    } else {
        fprintf(stderr, "Error: Invalid color object\n");
        return -1;
    }
}




HSL rgbToHsl(int r, int g, int b) {
    float r_f = r / 255.0f;
    float g_f = g / 255.0f;
    float b_f = b / 255.0f;

    // Difference between max and min, used for saturation and hue
    float max_val = fmax(fmax(r_f, g_f), b_f);
    float min_val = fmin(fmin(r_f, g_f), b_f);
    float delta = max_val - min_val;

    HSL hsl;

    // Calculate lightness as the average of max and min
    hsl.l = (max_val + min_val) / 2.0f;

    // If all components are equal, the color is achromatic (no hue)
    if (delta == 0.0f) {
        hsl.h = 0.0f;
        hsl.s = 0.0f;
    } else {
    // Compute saturation based on the lightness
        hsl.s = hsl.l > 0.5f ? delta / (2.0f - max_val - min_val) : delta / (max_val + min_val);

        // Compute hue based on which RGB component is the max
        if (max_val == r_f)
            hsl.h = fmod((g_f - b_f) / delta + (g_f < b_f ? 6.0f : 0.0f), 6.0f);
        else if (max_val == g_f)
            hsl.h = (b_f - r_f) / delta + 2.0f;
        else
            hsl.h = (r_f - g_f) / delta + 4.0f;

        hsl.h *= 60.0f;
    }

    return hsl;
}

int compareColorExact(const void* a, const void* b){
    Color* colorA = (Color*)a;
    Color* colorB = (Color*)b;
    if (colorA == NULL || colorB == NULL) return -1;
    if ((colorA->r == colorB->r) && (colorA->b==colorB->b) && (colorA->g==colorB->g)) return 1;
    return 0;
}

int compareColorDist(const void* a, const void* b){
    Color* colorA = (Color*)a;
    Color* colorB = (Color*)b;
    if (colorA == NULL || colorB == NULL) return -1;
   // if ((colorA->r == colorB->r) && (colorA->b==colorB->b) && (colorA->g==colorB->g)) return 0;

    HSL colorA_HSL = rgbToHsl(colorA->r, colorA->g, colorA->b);
    HSL colorB_HSL = rgbToHsl(colorB->r, colorB->g, colorB->b);

    double distance = abs(colorA_HSL.h - colorB_HSL.h);
    return (int) distance;
}

void free_color(void* this_color) {
    if (this_color != NULL) {
        free(this_color);
    }
}