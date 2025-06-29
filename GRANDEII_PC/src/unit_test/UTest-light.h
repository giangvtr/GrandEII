//
// Created by giangvu on 3/15/25.
//

#ifndef UTEST_LIGHT_H
#define UTEST_LIGHT_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include "../model/function.h"

int setup_light(void **state);
int teardown_light(void **state);
int setup_empty_light(void **state);

void test_alloc_light(void **state);
void test_initNewLight(void **state);
void test_initNewLight_NULL(void **state);

void test_setColor(void **state);
void test_setColor_NULL(void **state);
void test_setColor_colorNotInList(void **state);

void test_setBrightness_valid(void **state);
void test_setBrightness_invalid(void **state);
void test_setBrightness_NULL(void **state);

void test_setLightName_null(void **state); //null name or null light
void test_setLightName_valid(void **state);

void test_getLightId(void **state);
void test_getLightId_NULL(void **state);

void test_getLightName(void **state);
void test_getLightName_NULL(void **state);

void test_deleteLight_null(void **state);
void test_printLight(void **state);
void test_printLight_lightNull(void **state);

void test_compareLight_null_inputs(void **state);
void test_compareLight_negative_id(void **state);
void test_compareLight_different_id(void **state);
void test_compareLight_same_id(void **state);

void test_getNearestColor_light_null(void **state);
void test_getNearestColor_color_null(void **state);
void test_getNearestColor_empty_list(void **state);
void test_getNearestColor_finds_nearest(void **state);

void test_getFunction_light_null(void **state);
void test_getFunction_empty_list(void **state);
void test_getFunction_finds_function(void **state);
#endif //UTEST_LIGHT_H
