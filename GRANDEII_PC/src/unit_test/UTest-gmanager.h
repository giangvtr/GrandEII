//
// Created by giangvu on 5/29/25.
//

#ifndef UTEST_GMANAGER_H
#define UTEST_GMANAGER_H

#include "../model/group_manager.h"
#include "../model/group.h"

int setup_manager(void **state);
int setup_manager_full(void **state);
int teardown_manager(void **state);

/* Test functions */
void test_createGroupManager_valid(void **state);
void test_createGroupManager_invalid(void **state);

void test_addEmptyGroup_success(void **state);
void test_addEmptyGroup_failure(void **state);

void test_assignGroupToManager_success(void **state);
void test_assignGroupToManager_failure(void **state);

void test_removeGroup_success(void **state);
void test_removeGroup_failure(void **state);

void test_printGroupManager(void **state);
void test_printGroupManager_no_groups(void **state);

void test_deleteGroupManager(void **state);

#endif //UTEST_GMANAGER_H
