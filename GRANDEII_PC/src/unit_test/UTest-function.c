//
// Created by giangvu on 5/28/25.
//

#include "UTest-function.h"

int setup_function(void **state) {
    Function * function = alloc_function();
    if (function == NULL) return -1;
    *state = function;
    return 0;
}

int teardown_function(void **state) {
    Function * function = (Function*) *state;
    free_function(function);
    return 0;
}

//======= TESTS =========
void test_alloc_function(void **state) {
    (void)state;
    Function *f = alloc_function();
    assert_non_null(f);
    free(f);
}

// Test set_funct with valid input
void test_set_funct_valid(void **state) {
    Function *f = *state;
    assert_int_equal(set_funct(f, FLASH), 0);
    assert_int_equal(f->name, FLASH);
}

// Test set_funct with NULL pointer
void test_set_funct_null(void **state) {
    (void)state;
    assert_int_equal(set_funct(NULL, ON), -1);
}

// Test print_function_info with valid function
void test_print_function_info(void **state) {
    Function *f = *state;
    set_funct(f, BRIGHTNESS_MORE);

    // Redirect stdout to buffer
    FILE *orig_stdout = stdout;
    char buffer[100];
    stdout = fmemopen(buffer, sizeof(buffer), "w");

    print_function_info(f);

    fflush(stdout);
    stdout = orig_stdout;
    assert_non_null(strstr(buffer, "Function name:"));
}

// Test print_function_info with NULL
void test_print_function_info_null(void **state) {
    (void)state;
    // Should not crash
    print_function_info(NULL);
    assert_true(1);
}

// Test compareFunction with equal functions
void test_compareFunction_equal(void **state) {
    Function *f1 = alloc_function();
    Function *f2 = alloc_function();
    set_funct(f1, JUMP);
    set_funct(f2, JUMP);
    assert_int_equal(compareFunction(f1, f2), 1);
    free(f1);
    free(f2);
}

// Test compareFunction with different functions
void test_compareFunction_not_equal(void **state) {
    Function *f1 = alloc_function();
    Function *f2 = alloc_function();
    set_funct(f1, ON);
    set_funct(f2, OFF);
    assert_int_equal(compareFunction(f1, f2), 0);
    free(f1);
    free(f2);
}

// Test compareFunction with NULL pointers
void test_compareFunction_null(void **state) {
    Function *f = *state;
    set_funct(f, STROBE);
    assert_int_equal(compareFunction(NULL, f), 0);
    assert_int_equal(compareFunction(f, NULL), 0);
    assert_int_equal(compareFunction(NULL, NULL), 0);
}
