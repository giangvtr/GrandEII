//
// Created by maxime on 30/05/25.
//

#include "UTest-json_parser.h"
#include "../../src/model/json_parser.h"
#include <stdio.h>
#include <stdlib.h>

// TEST_DATA_PATH is set in CMakeLists.txt, so the tests work from any directory
static const char *valid_path = TEST_DATA_PATH "/valid_device.json";
static const char *invalid_group_path = TEST_DATA_PATH "/invalid_no_groups.json";
static const char *malformed_path = TEST_DATA_PATH "/malformed.json";
static const char *no_file_path = TEST_DATA_PATH "/nonexistent.json";
static const char *multiple_groups_path = TEST_DATA_PATH "/multiple_groups.json";
static const char *no_lights_path = TEST_DATA_PATH "/no_lights.json";

// deleteGroup frees the group itself
static void free_groups(Group **groups, int nbGroups) {
    for (int i = 0; i < nbGroups; i++) deleteGroup(groups[i]);
    free(groups);
}

void test_parse_valid_file(void **state) {
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(valid_path, &nbGroups);
    assert_non_null(groups);
    assert_int_equal(nbGroups, 1);
    assert_non_null(groups[0]);
    assert_string_equal(groups[0]->name, "Group 1");
    assert_int_equal(groups[0]->nbLights, 1);
    free_groups(groups, nbGroups);
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
    assert_int_equal(nbGroups, -1);
}

void test_parse_nonexistent_file(void **state){
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(no_file_path, &nbGroups);
    assert_null(groups);
    assert_int_equal(nbGroups, -1);
}

void test_parse_multiple_groups(void **state){
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(multiple_groups_path, &nbGroups);
    assert_non_null(groups);
    assert_int_equal(nbGroups, 2);
    assert_string_equal(groups[0]->name, "Group 1");
    assert_string_equal(groups[1]->name, "Group 2");
    free_groups(groups, nbGroups);
}

void test_parse_group_with_no_lights(void **state){
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(no_lights_path, &nbGroups);
    assert_non_null(groups);
    assert_int_equal(nbGroups, 1);
    assert_non_null(groups[0]);
    assert_int_equal(groups[0]->nbLights, 0);
    free_groups(groups, nbGroups);
}

void test_parse_lights_colors(void **state){
    (void)state;
    int nbGroups = 0;
    Group **groups = parse_device_file(valid_path, &nbGroups);
    assert_non_null(groups);
    assert_int_equal(nbGroups, 1);
    assert_non_null(groups[0]);
    assert_non_null(groups[0]->lights);

    setOnFirst(groups[0]->lights);
    Light *light = (Light *)getCurrentData(groups[0]->lights);
    assert_non_null(light);
    assert_string_equal(light->name, "Light 1");
    assert_int_equal(nbElement(light->color_list), 1);
    assert_int_equal(nbElement(light->function_list), 1);

    setOnFirst(light->color_list);
    Color *color = (Color *)getCurrentData(light->color_list);
    assert_non_null(color);
    assert_int_equal(color->r, 255);
    free_groups(groups, nbGroups);
}
