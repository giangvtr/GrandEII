//
// Tests of the allocation-failure branches, using mock_alloc.
// Each mock_alloc_fail_after() is armed right before the call under test.
// Allocation counts: allocGroup = 1, initGroup = 2 (name, light list).
//

#include "UTest-alloc_fail.h"
#include "mock_alloc.h"
#include "../model/color.h"
#include "../model/function.h"
#include "../model/group.h"
#include "../model/group_manager.h"
#include "../model/json_parser.h"
#include "../model/light.h"
#include "../model/list_common.h"
#include <stdlib.h>

void test_alloc_color_fails(void **state) {
    (void)state;
    mock_alloc_fail_after(0);
    assert_null(alloc_color());
    mock_alloc_reset();
}

void test_alloc_function_fails(void **state) {
    (void)state;
    mock_alloc_fail_after(0);
    assert_null(alloc_function());
    mock_alloc_reset();
}

void test_allocGroup_fails(void **state) {
    (void)state;
    mock_alloc_fail_after(0);
    assert_null(allocGroup());
    mock_alloc_reset();
}

void test_allocList_fails(void **state) {
    (void)state;
    mock_alloc_fail_after(0);
    assert_null(allocList());
    mock_alloc_reset();
}

void test_alloc_light_fails(void **state) {
    (void)state;
    // light, then color list, then function list
    for (int failing = 0; failing < 3; failing++) {
        mock_alloc_fail_after(failing);
        assert_null(alloc_light());
        mock_alloc_reset();
    }
}

void test_createGroupManager_groups_array_fails(void **state) {
    (void)state;
    // manager, then groups array
    mock_alloc_fail_after(1);
    assert_null(createGroupManager(3));
    mock_alloc_reset();
}

void test_createGroupManager_group_name_fails(void **state) {
    (void)state;
    // manager, groups array, group 0 (allocGroup), then its name
    mock_alloc_fail_after(3);
    assert_null(createGroupManager(3));
    mock_alloc_reset();
}

void test_createGroupManager_second_group_fails(void **state) {
    (void)state;
    // manager, groups array, group 0 (3 allocations), then group 1 allocGroup:
    // the already created group 0 must be cleaned up
    mock_alloc_fail_after(5);
    assert_null(createGroupManager(3));
    mock_alloc_reset();
}

void test_addEmptyGroup_alloc_fails(void **state) {
    (void)state;
    GroupManager *manager = createGroupManager(2);
    assert_non_null(manager);

    mock_alloc_fail_after(0);
    assert_int_equal(addEmptyGroup(manager), -1);
    mock_alloc_reset();

    assert_int_equal(manager->numGroups, 2);
    deleteGroupManager(manager);
}

void test_addEmptyGroup_init_fails(void **state) {
    (void)state;
    GroupManager *manager = createGroupManager(2);
    assert_non_null(manager);

    // allocGroup succeeds, initGroup's name allocation fails
    mock_alloc_fail_after(1);
    assert_int_equal(addEmptyGroup(manager), -1);
    mock_alloc_reset();

    assert_int_equal(manager->numGroups, 2);
    deleteGroupManager(manager);
}

void test_assignGroupToManager_rename_fails(void **state) {
    (void)state;
    GroupManager *manager = createGroupManager(2);
    assert_non_null(manager);

    Group *newGroup = allocGroup();
    Color color = {100, 100, 100};
    initGroup(newGroup, "CustomGroup", 50, &color);

    // the renaming allocation is the first one made by assignGroupToManager
    mock_alloc_fail_after(0);
    assert_int_equal(assignGroupToManager(manager, newGroup), -1);
    mock_alloc_reset();

    // the manager owns newGroup now and frees it
    deleteGroupManager(manager);
}

void test_parse_device_file_buffer_alloc_fails(void **state) {
    (void)state;
    int nbGroups = 0;
    // the file buffer is the first allocation made by parse_device_file
    mock_alloc_fail_after(0);
    Group **groups = parse_device_file(TEST_DATA_PATH "/valid_device.json", &nbGroups);
    mock_alloc_reset();

    assert_null(groups);
    assert_int_equal(nbGroups, -1);
}
