//
// Tests of the allocation-failure branches, using mock_alloc.
//

#ifndef UTEST_ALLOC_FAIL_H
#define UTEST_ALLOC_FAIL_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_alloc_color_fails(void **state);
void test_alloc_function_fails(void **state);
void test_allocGroup_fails(void **state);
void test_allocList_fails(void **state);
void test_alloc_light_fails(void **state);
void test_createGroupManager_groups_array_fails(void **state);
void test_createGroupManager_group_name_fails(void **state);
void test_createGroupManager_second_group_fails(void **state);
void test_addEmptyGroup_alloc_fails(void **state);
void test_addEmptyGroup_init_fails(void **state);
void test_assignGroupToManager_rename_fails(void **state);
void test_parse_device_file_buffer_alloc_fails(void **state);

#endif //UTEST_ALLOC_FAIL_H
