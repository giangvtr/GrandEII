//
// Created by giangvu on 3/15/25.
//

#ifndef UTEST_COLOR_H
#define UTEST_COLOR_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

int setup_color(void **state);
int teardown_color(void **state);

/**
 * @brief Test for init_color function
 * Ensures that init_color initializes a color correctly with given values.
 */
void test_init_color(void **state);

/**
 * @brief Test for init_color function in case the color pointer is NULL
 * Ensures that init_color initializes a color correctly with given values.
 */
void test_init_color_NULL(void **state);

/**
 * @brief Test for print_color_info function
 *
 * Ensures that print_color_info does not crash when given a valid Color.
 */
void test_print_color_info(void **state);
void test_print_color_info_NULL(void **state);

void test_rgbToHsl_basic_colors(void **state);

void test_compareColorExact_equal_colors(void **state);
void test_compareColorExact_equal_colors2(void **state);
void test_compareColorExact_different_colors(void **state);
void test_compareColorExact_null_input(void **state);

void test_compareColorDist_typical_colors(void **state);
void test_compareColorDist_null_input(void **state);


#endif //UTEST_COLOR_H
