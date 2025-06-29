//
// Created by giangvu on 4/6/25.
//

#ifndef UTEST_LIST_H
#define UTEST_LIST_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include "../model/list_common.h"
#include "../model/light.h"
#include "../model/color.h"


//Group setup_color-teardown_color test environment
int setup_list_light(void **state);
int setup_list_light_2light(void **state);
int setup_list_color(void **state);
int teardown_list_light(void **state);
int teardown_list_color(void **state);

//Group 1: Create a list and basic checks
void test_initList(void **state);
void test_initList_null(void **state);
void test_printList_light(void **state);

void test_isEmpty_empty(void **state);
void test_isEmpty_notEmpty(void **state);

void test_isFirst_empty(void **state);
void test_isFirst_1element(void **state);

void test_isLast_empty(void **state);

void test_isOutOfList_empty(void **state);
void test_isOutOfList_out(void **state);
void test_isOutOfList_notEmpty_in(void **state);

void test_nbElement_light(void **state);
void test_getCurrentData_null(void **state);

//Group 2: Basic navigations
void test_setOnFirst_empty(void **state);
void test_setOnPrev_light(void **state);
void test_setOnPrev_light(void **state);

//Group 3: Modify List
void test_insertFirst_notFound(void **state);
void test_insertFirst_wrong_type(void **state);
void test_insertFirst_duplicate(void **state);

void test_printList_light(void **state);
void test_printList_null(void **state);

void test_nbElement_light(void **state);

void test_find_light(void **state);

void test_find_min_light(void **state);
void test_find_min_null(void **state);



//Group 4: Delete and exit
void test_deleteList_empty(void **state);
void test_deleteValue_not_found(void **state);
void test_deleteValue_found(void **state);
void test_deleteValue_null(void **state);







#endif //UTEST_LIST_H
