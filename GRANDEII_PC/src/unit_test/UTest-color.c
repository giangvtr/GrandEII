//
// Created by giangvu on 3/15/25.
//

#include "UTest-color.h"
#include "../model/color.h"
#include <math.h>


int setup_color(void **state) {
    // Provide memory for setup_color's alloc_colo
    Color *color = alloc_color();
    if (color == NULL) return -1;
    *state = color;
    return 0;
}

int teardown_color(void **state) {
    Color *color = (Color *)*state;
    free_color(color);
    return 0;
}

void test_init_color(void **state) {
    Color *color = (Color *)*state;
    uint8_t r = 100, g = 150, b = 200;
    set_color(color, r, g, b);
    assert_int_equal(color->r, r);
    assert_int_equal(color->g, g);
    assert_int_equal(color->b, b);
}

void test_init_color_NULL(void **state) {
    uint8_t r = 100, g = 150, b = 200;
    assert_int_equal(set_color(NULL, r, g, b),-1);
}

void test_print_color_info_NULL(void **state) {
    assert_int_equal(print_color_info(NULL),-1);
}

void test_print_color_info(void **state) {
    Color *color = (Color *)*state;
    set_color(color, 50, 100, 150);
    print_color_info(color);
}

void test_rgbToHsl_basic_colors(void **state) {
    // Black (0,0,0) -> (0°, 0, 0)
    HSL black = rgbToHsl(0, 0, 0);
    assert_float_equal(black.h, 0.0, 0.01);
    assert_float_equal(black.s, 0.0, 0.01);
    assert_float_equal(black.l, 0.0, 0.01);

    // White (255,255,255) -> (0°, 0, 1)
    HSL white = rgbToHsl(255, 255, 255);
    assert_float_equal(white.h, 0.0, 0.01);
    assert_float_equal(white.s, 0.0, 0.01);
    assert_float_equal(white.l, 1.0, 0.01);

    // Red (255,0,0) -> (0°, 1, 0.5)
    HSL red = rgbToHsl(255, 0, 0);
    assert_float_equal(red.h, 0.0, 0.01);
    assert_float_equal(red.s, 1.0, 0.01);
    assert_float_equal(red.l, 0.5, 0.01);

    // Green (0,255,0) -> (120°, 1, 0.5)
    HSL green = rgbToHsl(0, 255, 0);
    assert_float_equal(green.h, 120.0, 0.01);
    assert_float_equal(green.s, 1.0, 0.01);
    assert_float_equal(green.l, 0.5, 0.01);

    // Blue (0,0,255) -> (240°, 1, 0.5)
    HSL blue = rgbToHsl(0, 0, 255);
    assert_float_equal(blue.h, 240.0, 0.01);
    assert_float_equal(blue.s, 1.0, 0.01);
    assert_float_equal(blue.l, 0.5, 0.01);

    // Gray (128,128,128) -> (0°, 0, 0.5)
    HSL gray = rgbToHsl(128, 128, 128);
    assert_float_equal(gray.h, 0.0, 0.01);
    assert_float_equal(gray.s, 0.0, 0.01);
    assert_float_equal(gray.l, 0.50196, 0.01); // 128/255 ≈ 0.50196

    HSL pink = rgbToHsl(255, 192, 203);
    assert_true(pink.l > 0.5);
    assert_float_equal(pink.s, (1.0 - 0.752) / (2.0 - 1.0 - 0.752), 0.01);  // s ≈ 0.542

    HSL redish = rgbToHsl(255, 50, 100);
    assert_float_equal(redish.h, fmod((0.196 - 0.392) / (1.0 - 0.196) + 6.0, 6.0) * 60.0, 1.0);

    HSL dark_orange = rgbToHsl(128, 64, 0);
    assert_true(dark_orange.l < 0.5);
    assert_float_equal(dark_orange.s, 1.0, 0.01);  // max=0.502, min=0, delta=0.502, s=1.0
}

void test_compareColorExact_equal_colors(void **state) {
    (void)state;
    Color color1 = {255, 100, 50};
    Color color2 = {255, 100, 50};

    int result = compareColorExact(&color1, &color2);
    assert_int_equal(result, 1);
}

void test_compareColorExact_different_colors(void **state) {
    (void)state;
    Color color1 = {255, 100, 50};
    Color color2 = {255, 100, 51};

    int result = compareColorExact(&color1, &color2);
    assert_int_equal(result, 0);
}

void test_compareColorExact_equal_colors2(void **state) {
    (void)state;
    Color color1 = {255, 101, 50};
    Color color2 = {255, 100, 50};
    int result = compareColorExact(&color1, &color2);
    assert_int_equal(result, 0);
}

void test_compareColorExact_null_input(void **state) {
    (void)state;
    Color color = {255, 100, 50};

    assert_int_equal(compareColorExact(NULL, &color), -1);
    assert_int_equal(compareColorExact(&color, NULL), -1);
    assert_int_equal(compareColorExact(NULL, NULL), -1);
}

void test_compareColorDist_typical_colors(void **state) {
    (void) state;
    Color red   = {255, 0, 0};    // hue ~0°
    Color green = {0, 255, 0};    // hue ~120°
    Color blue  = {0, 0, 255};    // hue ~240°

    assert_int_equal(compareColorDist(&red, &green), 120);
    assert_int_equal(compareColorDist(&green, &blue), 120);
    assert_int_equal(compareColorDist(&red, &blue), 240);
}

void test_compareColorDist_null_input(void **state) {
    (void) state;
    Color color = {128, 64, 32};
    assert_int_equal(compareColorDist(NULL, &color), -1);
    assert_int_equal(compareColorDist(&color, NULL), -1);
    assert_int_equal(compareColorDist(NULL, NULL), -1);
}