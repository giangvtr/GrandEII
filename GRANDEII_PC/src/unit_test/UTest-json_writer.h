//
// Created by maxime on 30/05/25.
//

#ifndef UTEST_JSON_WRITER_H
#define UTEST_JSON_WRITER_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_write_device_with_one_group(void **state);
void test_write_device_with_empty_group(void **state);
void test_write_device_with_no_groups(void **state);

#endif //UTEST_JSON_WRITER_H
