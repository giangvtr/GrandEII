//
// Created by maxime on 30/05/25.
//

#include "../model/json_writer.h"
#include "../model/group.h"
#include "../model/light.h"
#include "../model/color.h"
#include "../model/function.h"
#include "../model/list_common.h"
#include "../model/lib/cJSON/cJSON.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../model/group_manager.h"
#include "UTest-json_writer.h"

static int compare_wrapper_color(void *a, void *b){
      return compareColorExact((const void *)a,(const void *)b);
}

static int compare_wrapper_function(void *a, void *b){
      return compareFunction((const void *)a,(const void *)b);
}

// The color is owned by the light's color list when with_colors is set
static Group *create_dummy_group(const char *group_name, int with_colors, int with_functions) {
    Light *light = alloc_light();
    initNewLight(light, "Light 1", 1);

    Color *group_color = NULL;
    if (with_colors) {
        Color *color = alloc_color();
        set_color(color, 255, 0, 0);
        INT_to_IR_Code(123456, color->ir_code);
        insertFirst(light->color_list, color, LIST_TYPE_COLOR, compare_wrapper_color);
        group_color = color;
    }

    if (with_functions) {
        Function *f = alloc_function();
        f->name = 1;
        INT_to_IR_Code(789, f->ir_code);
        insertFirst(light->function_list, f, LIST_TYPE_FUNCTION, compare_wrapper_function);
    }
    Group *g = allocGroup();
    g->name = strdup(group_name);
    g->lights = allocList();
    initList(g->lights, LIST_TYPE_LIGHT);
    insertFirst(g->lights, light, LIST_TYPE_LIGHT, compareLight);
    g->nbLights = 1;
    g->group_brightness = 80;
    g->group_color = group_color;
    return g;
}

// Reads and parses a JSON file, then deletes it. The caller must cJSON_Delete the result.
static cJSON *read_and_remove_json(const char *filename) {
    FILE *f = fopen(filename, "r");
    assert_non_null(f);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *content = malloc(size + 1);
    size_t nbRead = fread(content, 1, size, f);
    content[nbRead] = 0;
    fclose(f);
    remove(filename);

    cJSON *root = cJSON_Parse(content);
    free(content);
    assert_non_null(root);
    return root;
}

void test_write_device_with_one_group(void **state) {
    (void)state;
    const char *filename = "test_output_1.json";
    Group *group = create_dummy_group("Group 1", 1, 1);
    Group *groups[] = { group };
    write_device_file((const Group **)groups, 1, filename);

    cJSON *root = read_and_remove_json(filename);
    assert_int_equal(cJSON_GetObjectItem(root, "nbGroups")->valueint, 1);

    cJSON *group_json = cJSON_GetObjectItem(root, "0");
    assert_non_null(group_json);
    assert_string_equal(cJSON_GetObjectItem(group_json, "name")->valuestring, "Group 1");
    assert_int_equal(cJSON_GetObjectItem(group_json, "nbLights")->valueint, 1);

    cJSON *light_json = cJSON_GetArrayItem(cJSON_GetObjectItem(group_json, "Lights"), 0);
    assert_non_null(light_json);
    assert_string_equal(cJSON_GetObjectItem(light_json, "name")->valuestring, "Light 1");
    assert_int_equal(cJSON_GetObjectItem(light_json, "nbCouleurs")->valueint, 1);
    assert_int_equal(cJSON_GetObjectItem(light_json, "nbFonctions")->valueint, 1);

    cJSON *color_json = cJSON_GetArrayItem(cJSON_GetObjectItem(light_json, "Couleurs"), 0);
    assert_int_equal(cJSON_GetObjectItem(color_json, "r")->valueint, 255);

    unsigned char expected_ir[4];
    INT_to_IR_Code(123456, expected_ir);
    assert_int_equal(cJSON_GetObjectItem(color_json, "IR Code")->valueint,
                     IR_Code_to_INT(expected_ir));

    cJSON_Delete(root);
    deleteGroup(group);
}

void test_write_device_with_empty_group(void **state) {
    (void)state;
    const char *filename = "test_output_empty.json";
    Group *g = allocGroup();
    g->name = strdup("Empty Group");
    g->lights = allocList();
    initList(g->lights, LIST_TYPE_LIGHT);
    g->nbLights = 0;
    g->group_color = NULL;
    Group *groups[] = {g};
    write_device_file((const Group **)groups, 1, filename);

    cJSON *root = read_and_remove_json(filename);
    cJSON *group_json = cJSON_GetObjectItem(root, "0");
    assert_non_null(group_json);
    assert_int_equal(cJSON_GetObjectItem(group_json, "nbLights")->valueint, 0);
    cJSON_Delete(root);
    deleteGroup(g);
}

void test_write_device_with_no_groups(void **state) {
    (void)state;
    const char *filename = "test_output_nogroup.json";
    write_device_file(NULL, 0, filename);

    cJSON *root = read_and_remove_json(filename);
    assert_int_equal(cJSON_GetObjectItem(root, "nbGroups")->valueint, 0);
    cJSON_Delete(root);
}

void test_write_device_with_no_colors_or_functions(void **state) {
    (void)state;
    const char *filename = "test_output_plain.json";
    Group *group = create_dummy_group("Plain", 0, 0);
    Group *groups[] = { group };
    write_device_file((const Group **)groups, 1, filename);

    cJSON *root = read_and_remove_json(filename);
    cJSON *light_json = cJSON_GetArrayItem(
        cJSON_GetObjectItem(cJSON_GetObjectItem(root, "0"), "Lights"), 0);
    assert_int_equal(cJSON_GetObjectItem(light_json, "nbCouleurs")->valueint, 0);
    assert_int_equal(cJSON_GetObjectItem(light_json, "nbFonctions")->valueint, 0);
    cJSON_Delete(root);
    deleteGroup(group);
}
