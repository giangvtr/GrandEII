//
// Created by giangvu on 5/28/25.
//

#ifndef UTEST_FUNCTION_H
#define UTEST_FUNCTION_H

#include <stddef.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>

#include "../model/function.h"

int setup_function(void **state);
int teardown_function(void **state);

void test_alloc_function(void **state);
void test_set_funct_valid(void **state);
void test_set_funct_null(void **state);
void test_print_function_info(void **state);
void test_print_function_info_null(void **state);
void test_compareFunction_equal(void **state);
void test_compareFunction_not_equal(void **state);
void test_compareFunction_null(void **state);


#endif //UTEST_FUNCTION_H
