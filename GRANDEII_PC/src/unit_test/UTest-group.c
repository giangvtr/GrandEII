//
// Created by giangvu on 5/28/25.
//

#include "UTest-group.h"

int setup_group(void **state) {
    Group *group = allocGroup();
    if (group == NULL) return -1;
    *state = group;
    return 0;
}

int teardown_group(void **state) {
    Group *group = (Group*)*state;
    deleteGroup(group); //Free all lights in the lists, the free list of lights
    return 0;
}

//======= TESTS =========

void test_initGroup_valid(void **state) {
    Group *group = *state;
    Color color = {255, 0, 0};
    assert_int_equal(initGroup(group, "TestGroup", 128, &color), 0);
    assert_string_equal(group->name, "TestGroup");
    assert_int_equal(group->group_brightness, 128);
    assert_ptr_equal(group->group_color, &color);
}

void test_initGroup_null(void **state) {
    (void)state;
    Color color = {0,0,0};
    assert_int_equal(initGroup(NULL, "Test", 100, &color), -1);
    Group *group = (Group*)*state;
    assert_int_equal(initGroup(group, NULL, 100, &color),-1);
}

void test_addLighttoGroup_valid(void **state) {
    Group *group = (Group*)*state;
    Color color = {0,0,0};
    initGroup(group, "Group", 100, &color);
    Light *light = alloc_light();
    initNewLight(light, "Light", INFRAROUGE);
    assert_int_equal(addLighttoGroup(group, light), 0);
    assert_int_equal(group->nbLights, 1);
    // Tear down will free lights
}

void test_addLighttoGroup_null(void **state) {
    Group *group = *state;
    assert_int_equal(addLighttoGroup(NULL, NULL), -1);
    assert_int_equal(addLighttoGroup(group, NULL), -1);
}

void test_addLighttoGroup_invalid(void **state) {
    Group *group = (Group*)*state;
    Color color = {0, 0, 0};
    initGroup(group, "Group", 100, &color);

    // Fill the group to MAX_LIGHTS
    for (int i = 0; i < MAX_LIGHTS; i++) {
        Light *light = alloc_light();
        initNewLight(light, "Light", INFRAROUGE);
        int ret = addLighttoGroup(group, light);
        assert_int_equal(ret, 0);
    }

    // Now group is full, adding one more light should fail
    Light *extra_light = alloc_light();
    initNewLight(extra_light, "ExtraLight", INFRAROUGE);
    int ret = addLighttoGroup(group, extra_light);
    assert_int_equal(ret, -1);

    deleteLight(extra_light);
}

void test_addLighttoGroup_insertFirst_fail(void **state) {
    Group *group = (Group*)*state;
    Light *light = alloc_light();
    initNewLight(light, "Light", INFRAROUGE);

    // Simulate failure by nullifying the list pointer
    free(group->lights);
    group->lights = NULL;

    int ret = addLighttoGroup(group, light);
    assert_int_equal(ret, -1);

    // Clean up
    deleteLight(light);
}

void test_isEmptyGroup_empty(void **state) {
    Group *group = *state;
    Color color = {0,0,0};
    initGroup(group, "Group", 100, &color);
    assert_int_equal(isEmptyGroup(group), 1);
}

void test_isEmptyGroup_notEmpty(void **state) {
    Group *group = *state;
    Color color = {0, 0, 0};
    initGroup(group, "Group", 100, &color);

    Light* test = alloc_light();
    initNewLight(test, "TestLight", INFRAROUGE);

    assert_int_equal(addLighttoGroup(group, test), 0);
    assert_int_equal(isEmptyGroup(group), 0);
    assert_int_equal(isEmptyGroup(NULL), -1);
}

void test_setGroupBrightness(void **state) {
    Group *group = *state;
    Color color = {0,0,0};
    initGroup(group, "Group", 100, &color);
    setGroupBrightness(group, 200);
    assert_int_equal(group->group_brightness, 200);
}

void test_setGroupColor(void **state) {
    Group *group = *state;
    Color color1 = {0,0,0};
    Color color2 = {1,2,3};
    initGroup(group, "Group", 100, &color1);
    setGroupColor(group, &color2);
    assert_ptr_equal(group->group_color, &color2);
}

void test_getGroupName(void **state) {
    Group *group = *state;
    Color color = {0,0,0};
    initGroup(group, "GroupName", 100, &color);
    assert_string_equal(getGroupName(group), "GroupName");
    assert_null(getGroupName(NULL));
}

void test_getGroupID(void **state) {
    Group *group = *state;
    Color color = {0,0,0};
    initGroup(group, "Group", 100, &color);
    assert_int_not_equal(getGroupID(group), 0);
    assert_int_equal(getGroupID(NULL), 0);
}

void test_getGroupColor(void **state) {
    Group *group = *state;
    Color color = {5,6,7};
    initGroup(group, "Group", 100, &color);
    assert_ptr_equal(getGroupColor(group), &color);
    assert_null(getGroupColor(NULL));
}

void test_compareGroup_equal(void **state) {
    Group *group1 = allocGroup();
    Group *group2 = allocGroup();
    Color color = {0,0,0};
    initGroup(group1, "Same", 100, &color);
    initGroup(group2, "Same", 100, &color);
    group2->id = group1->id; // Force same ID
    assert_int_equal(compareGroup(group1, group2), 1);
    deleteGroup(group1);
    deleteGroup(group2);
}

void test_compareGroup_not_equal(void **state) {
    Group *group1 = allocGroup();
    Group *group2 = allocGroup();
    Color color = {0,0,0};
    initGroup(group1, "A", 100, &color);
    initGroup(group2, "B", 100, &color);
    assert_int_equal(compareGroup(group1, group2), 0);
    deleteGroup(group1);
    deleteGroup(group2);
}

void test_compareGroup_name_null(void **state) {
    Group group1 = { .id = 1, .name = NULL };
    Group group2 = { .id = 2, .name = NULL };

    // IDs differ, names NULL → should return 0 and hit last line
    assert_int_equal(compareGroup(&group1, &group2), 0);

    Group group3 = { .id = 3, .name = "Group3" };
    Group group4 = { .id = 4, .name = NULL };

    // One name NULL, IDs differ → should return 0
    assert_int_equal(compareGroup(&group3, &group4), 0);
}


void test_printGroup(void **state) {
    Group *group = *state;
    Color color = {10,20,30};
    initGroup(group, "PrintGroup", 150, &color);
    // Redirect stdout to buffer
    FILE *orig_stdout = stdout;
    char buffer[512];
    stdout = fmemopen(buffer, sizeof(buffer), "w");
    printGroup(group);
    fflush(stdout);
    stdout = orig_stdout;
    assert_non_null(strstr(buffer, "Group Information"));
    assert_int_equal(printGroup(NULL), -1);
    assert_non_null(strstr(buffer, "PrintGroup"));
    assert_int_equal(printGroup(group), 0);
}

void test_freeGroup_null(void **state) {
    (void)state;
    deleteGroup(NULL);
    assert_true(1);
}

void test_printGroup_color_none(void **state) {
    Group *group = *state;

    // Initialize group with NULL color
    Color *color = NULL;
    initGroup(group, "TestGroup", 50, color);

    // Add one light to have lights list non-empty
    Light *light = alloc_light();
    initNewLight(light, "Light1", INFRAROUGE);
    addLighttoGroup(group, light);

    // Call printGroup - should print "Color        : None"
    printGroup(group);
}

void test_printGroup_lights_present(void **state) {
    Group *group = *state;

    Color color = {255, 100, 50};
    initGroup(group, "TestGroup", 50, &color);

    // Add one light to group
    Light *light = alloc_light();
    initNewLight(light, "Light1", INFRAROUGE);
    addLighttoGroup(group, light);

    // Call printGroup - should print "Lights       :\n" and call printList
    printGroup(group);
}

