//
// Created by maxime on 30/05/25.
//

#include "../model/json_writer.h"
#include "../model/group.h"
#include "../model/light.h"
#include "../model/color.h"
#include "../model/function.h"
#include "../model/list_common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../model/group_manager.h"
#include "UTest-json_writer.h"

//void freeList(void *list);

static int compare_wrapper_color(void *a, void *b){
      return compareColorExact((const void *)a,(const void *)b);
}

static int compare_wrapper_function(void *a, void *b){
      return compareFunction((const void *)a,(const void *)b);
}

static void free_group(Group *g){
    if(!g) return;
    free(g->name);
    deleteList(g->lights, deleteLight);
    if(g->group_color) free(g->group_color);
    free(g);
}

static Group *create_dummy_group(const char *group_name, int with_colors, int with_functions) {
    Color *color = alloc_color();
    set_color(color, 255, 0, 0);
    INT_to_IR_Code(123456, color->ir_code);
    Light *light = alloc_light();
    initNewLight(light, "Light 1", 1);
    if (with_colors) {
        insertFirst(light->color_list, color, LIST_TYPE_COLOR, compare_wrapper_color);
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
    insertFirst(g->lights, light, LIST_TYPE_LIGHT, compareLight);
    g->nbLights = 1;
    g->group_brightness = 80;
    g->group_color = color;
    return g;
}

void test_write_device_with_one_group(void **state) {
    (void)state;
    const char *filename = "test_output_1.json";
    Group *group = create_dummy_group("Group 1", 1, 1);
    Group *groups[] = { group };
    write_device_file((const Group **)groups, 1, filename);
    FILE *f = fopen(filename, "r");
    assert_non_null(f);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *content = malloc(size + 1);
    fread(content, 1, size, f);
    content[size] = '\0';
    assert_non_null(strstr(content, "\"nbGroups\":1"));
    assert_non_null(strstr(content, "\"name\":\"Group 1\""));
    assert_non_null(strstr(content, "\"nbLights\":1"));
    assert_non_null(strstr(content, "\"name\":\"Light 1\""));
    assert_non_null(strstr(content, "\"nbCouleurs\":1"));
    assert_non_null(strstr(content, "\"nbFonctions\":1"));
    assert_non_null(strstr(content, "\"IR Code\":123456"));
    fclose(f);
    free(content);
    free(group);
}

void test_write_device_with_empty_group(void **state) {
    (void)state;
    const char *filename = "test_output_empty.json";
    Group *g = allocGroup();
    g->name = strdup("Empty Group");
    g->lights = allocList();
    g->nbLights = 0;
    Group *groups[] = {g};
    write_device_file((const Group **)groups, 1, filename);
    FILE *f = fopen(filename, "r");
    assert_non_null(f);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *content = malloc(size + 1);
    fread(content, 1, size, f);
    content[size] = '\0';
    assert_non_null(strstr(content, "\"nbLights\":0"));
    fclose(f);
    free(content);
    free(g);
}

void test_write_device_with_no_groups(void **state) {
    (void)state;
    const char *filename = "test_output_nogroup.json";
    write_device_file(NULL, 0, filename);
    FILE *f = fopen(filename, "r");
    assert_non_null(f);
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *content = malloc(size + 1);
    fread(content, 1, size, f);
    content[size] = '\0';
    assert_non_null(strstr(content, "\"nbGroups\":0"));
    fclose(f);
    free(content);
}