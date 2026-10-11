//
// Created by maxime on 30/05/25.
//

#ifndef UTEST_JSON_PARSER_H
#define UTEST_JSON_PARSER_H

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_parse_valid_file(void **state);
void test_parse_invalid_group_count(void **state);
void test_parse_malformed_json(void **state);
void test_parse_nonexistent_file(void **state);
void test_parse_multiple_groups(void **state);
void test_parse_group_with_no_lights(void **state);
void test_parse_lights_colors(void **state);
void test_parse_missing_nbgroups(void **state);
void test_parse_group_missing_name(void **state);

#endif //UTEST_JSON_PARSER_H
