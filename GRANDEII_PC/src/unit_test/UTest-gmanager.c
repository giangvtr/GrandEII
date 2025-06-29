//
// Created by giangvu on 5/29/25.
//

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "UTest-gmanager.h"

int setup_manager(void **state) {
    GroupManager *manager = createGroupManager(2);
    if (manager == NULL) return -1;
    *state = manager;
    return 0;
}

int setup_manager_full(void **state) {
    GroupManager *manager = createGroupManager(3);
    if (manager == NULL) return -1;
    Light * l1 = alloc_light();
    initNewLight(l1,"L1", INFRAROUGE);
    Light * l2 = alloc_light();
    initNewLight(l2,"L2", INFRAROUGE);
    Light * l3 = alloc_light();
    initNewLight(l3,"L3", INFRAROUGE);

    addLighttoGroup(manager->groups[0],l1);
    addLighttoGroup(manager->groups[1],l2);
    addLighttoGroup(manager->groups[2],l3);
    *state = manager;
    return 0;
}

// Teardown: delete the GroupManager and free resources
int teardown_manager(void **state) {
    GroupManager *manager = (GroupManager *)*state;
    if (manager != NULL) {
        deleteGroupManager(manager);
    }
    return 0;
}

void test_createGroupManager_valid(void **state) {
    (void)state;
    GroupManager *manager = createGroupManager(3);
    assert_non_null(manager);
    assert_int_equal(manager->numGroups, 3);
    deleteGroupManager(manager);
}

void test_createGroupManager_invalid(void **state) {
    (void)state;
    // Too few groups
    GroupManager *mgr1 = createGroupManager(1);
    assert_null(mgr1);

    // Too many groups
    GroupManager *mgr2 = createGroupManager(MAX_GROUP + 1);
    assert_null(mgr2);
}

void test_addEmptyGroup_success(void **state) {
    GroupManager *manager = *state;
    int initialCount = manager->numGroups;

    int ret = addEmptyGroup(manager);
    assert_int_equal(ret, 1);
    assert_int_equal(manager->numGroups, initialCount + 1);
}

void test_addEmptyGroup_failure(void **state) {
    GroupManager *manager = *state;
    // Fill manager to MAX_GROUP
    while (manager->numGroups < MAX_GROUP) {
        addEmptyGroup(manager);
    }
    // Now adding should fail
    int ret = addEmptyGroup(manager);
    assert_int_equal(ret, -1);
    assert_int_equal(addEmptyGroup(NULL), -1);
}

void test_assignGroupToManager_success(void **state) {
    GroupManager *manager = *state;

    Group *newGroup = allocGroup();
    Color color = {100, 100, 100};
    initGroup(newGroup, "CustomGroup", 50, &color);

    assert_int_equal(assignGroupToManager(manager, newGroup), 1);
    assert_string_equal(manager->groups[0]->name, "Group 1");
}

void test_assignGroupToManager_failure(void **state) {
    GroupManager *manager = *state;
    Group *newGroup = allocGroup();
    Color color = {100, 100, 100};
    initGroup(newGroup, "CustomGroup", 50, &color);
    assert_int_equal(assignGroupToManager(manager, newGroup), -1);
    assert_int_equal(assignGroupToManager(NULL, NULL), -1);
    assert_int_equal(assignGroupToManager(manager, NULL), -1);
    assert_int_equal(assignGroupToManager(NULL, newGroup), -1);
    deleteGroup(newGroup);
}

void test_removeGroup_success(void **state) {
    GroupManager *manager = *state;

    // Assign a group to manager slot 0
    Group *groupToRemove = allocGroup();
    Color color = {10, 20, 30};
    initGroup(groupToRemove, "RemoveMe", 10, &color);
    assignGroupToManager(manager, groupToRemove);

    // Remove the group
    int ret = removeGroup(manager, groupToRemove);
    assert_int_equal(ret, 1);

    // After removal, numGroups should be decreased
    assert_int_equal(manager->numGroups, 1);
}

void test_removeGroup_failure(void **state) {
    GroupManager *manager = *state;

    Group *nonExistentGroup = allocGroup();
    initGroup(nonExistentGroup, "NonExistent", 0, NULL);

    int ret = removeGroup(manager, nonExistentGroup);
    assert_int_equal(ret, -1);
    assert_int_equal(removeGroup(NULL, nonExistentGroup), -1);
    deleteGroup(nonExistentGroup);
}

void test_printGroupManager(void **state) {
    GroupManager *manager = *state;
    printGroupManager(manager);
}

void test_printGroupManager_no_groups(void **state) {
    GroupManager *manager = *state;
    manager->numGroups = 0;
    int ret = printGroupManager(manager);
    manager->numGroups = 2;
    assert_int_equal(ret, -1);
}


void test_deleteGroupManager(void **state) {
    GroupManager *manager = *state;

    // Just call deleteGroupManager to ensure no crash and proper cleanup
    deleteGroupManager(manager);

    // Mark state as NULL to avoid double free in teardown
    *state = NULL;
}

