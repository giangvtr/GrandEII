//
// Created by giangvu on 5/28/25.
//

#ifndef UTEST_GROUP_H
#define UTEST_GROUP_H

#include <stddef.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>

#include "../model/group.h"

int setup_group(void **state);
int teardown_group(void **state);

void test_initGroup_valid(void **state);
void test_initGroup_null(void **state);
void test_addLighttoGroup_valid(void **state);
void test_addLighttoGroup_null(void **state);
void test_addLighttoGroup_invalid(void **state);
void test_addLighttoGroup_insertFirst_fail(void **state);

void test_isEmptyGroup_empty(void **state);
void test_isEmptyGroup_notEmpty(void **state);

void test_setGroupBrightness(void **state);
void test_setGroupColor(void **state);

void test_getGroupName(void **state);
void test_getGroupID(void **state);
void test_getGroupColor(void **state);

void test_compareGroup_equal(void **state);
void test_compareGroup_not_equal(void **state);
void test_compareGroup_name_null(void **state);

void test_printGroup(void **state);
void test_printGroup_color_none(void **state);
void test_printGroup_lights_present(void **state);

void test_freeGroup_null(void **state);

#endif //UTEST_GROUP_H
