//
// Created by maxime on 30/05/25.
//

#include "UTest-json_parser.h"
#include "../../src/model/json_parser.h"
#include <stdio.h>
#include <stdlib.h>

static const char *valid_path = "tests/data/valid_device.json";
static const char *invalid_group_path = "tests/data/invalid_no_groups.json";
static const char *malformed_path = "tests/data/malformed.json";
static const char *no_file_path = "tests/data/nonexistent.json";
static const char *multiple_groups_path = "tests/data/multiple_groups.json";
static const char *no_lights_path = "tests/data/no_lights.json";

void test_parse_valid_file(void **state) {
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(valid_path, &nbGroups);
    assert_non_null(groups);
    assert_int_equal(nbGroups, 1);
    assert_non_null(groups[0]);
    assert_string_equal(groups[0]->name, "Group 1");
    free(groups);
}

void test_parse_invalid_group_count(void **state) {
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(invalid_group_path, &nbGroups);
    assert_null(groups);
    assert_int_equal(nbGroups, -1);
}

void test_parse_malformed_json(void **state) {
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(malformed_path, &nbGroups);
    assert_null(groups);
}

void test_parse_nonexistent_file(void **state){
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(no_file_path, &nbGroups);
    assert_null(groups);
}


void test_parse_multiple_groups(void **state){
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(multiple_groups_path, &nbGroups);
    assert_non_null(groups);
    assert_int_equal(nbGroups, 2);
    assert_string_equal(groups[0]->name, "Group 1");
    assert_string_equal(groups[1]->name, "Group 2");
    free(groups);
}

void test_parse_group_with_no_lights(void **state){
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(no_lights_path, &nbGroups);
    assert_non_null(groups);
    assert_int_equal(nbGroups, 1);
    assert_non_null(groups[0]);
    assert_null(groups[0]->lights);
    free(groups);
}

void test_parse_lights_colors(void **state){
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(valid_path, &nbGroups);
    assert_non_null(groups);
    assert_int_equal(nbGroups, 1);
    assert_non_null(groups[0]);
    assert_non_null(groups[0]->lights);
    List *lightList = groups[0]->lights;
    assert_non_null(lightList);
    Light *light = (Light*)lightList;
    List *colors = light->color_list;
    assert_non_null(colors);
    Color *color = (Color *)colors;
    assert_int_equal(color->r, 255);
}