//
// Created by giangvu on 4/21/25.
//

/**
 * @file group_manager.h
 * @author giangvu
 * @date 2025-04-21
 * @brief Interface for managing multiple light groups in a lighting control system.
 *
 * This module defines the `GroupManager` structure and a set of functions to create, add,
 * assign, remove, and manage multiple `Group` instances. It acts as a high-level manager
 * coordinating the behavior and lifecycle of several groups of lights.
 *
 * @note This module builds upon the `Group` structure and functions defined in `group.h`.
 */

#ifndef GROUP_MANAGER_H
#define GROUP_MANAGER_H

#include "group.h"  /**< Includes definitions related to an individual Group. */

/// @def MAX_GROUP
/// @brief Maximum number of groups that can be managed simultaneously.
#define MAX_GROUP 3

/**
 * @struct GroupManager
 * @brief Structure representing a manager that handles multiple groups of lights.
 *
 * Each group can be independently configured and controlled. The manager provides
 * mechanisms to organize and maintain these groups.
 */
typedef struct {
    Group **groups;      /**< Array of groups */
    int numGroups;      /**< Current number of groups */
} GroupManager;

/**
 * @brief Creates a new GroupManager with a specified number of empty groups.
 *
 * Each group will be automatically initialized with a default name (e.g., "Group 1").
 * Groups are created using `allocGroup()` and initialized using `initGroup()`.
 *
 * @param numGroups Number of groups to initialize (must be > 1 and <= MAX_GROUP).
 * @return Pointer to a valid GroupManager on success, or NULL on error (invalid count or memory allocation failure).
 *
 * @see allocGroup
 * @see initGroup
 */
GroupManager *createGroupManager(int numGroups);

/**
 * @brief Adds a new empty group to an existing GroupManager.
 *
 * This dynamically resizes the manager’s internal array and creates a new group.
 * The group is initialized with a default name.
 *
 * @param manager Pointer to the GroupManager.
 * @return 1 on success, -1 on failure (e.g., NULL pointer, memory issue, or MAX_GROUP limit reached).
 *
 * @see allocGroup
 * @see initGroup
 */
int addEmptyGroup(GroupManager *manager);

/**
 * @brief Assigns a provided group to the first available empty group slot in the manager.
 *
 * The ownership of the passed `group` pointer is transferred to the manager.
 * Caller must not free or reuse the group after this call.
 * The new group will be renamed based on its assigned slot.
 *
 * @param manager Pointer to the GroupManager.
 * @param group Pointer to the Group to assign (must not be NULL).
 * @return 1 on success, -1 on failure (e.g., no empty slots or NULL pointers).
 *
 * @see isEmptyGroup
 * @see deleteGroup
 */
int assignGroupToManager(GroupManager *manager, Group *group);

/**
 * @brief Removes a specific group from the manager.
 *
 * The group to be removed is matched by comparing its contents (using `compareGroup`).
 * After deletion, remaining groups are shifted and renamed to maintain consistency.
 *
 * @param manager Pointer to the GroupManager.
 * @param toDelete Pointer to the group to be deleted (must not be NULL).
 * @return 1 on successful removal, -1 if the group was not found or input is invalid.
 *
 * @see compareGroup
 * @see deleteGroup
 */
int removeGroup(GroupManager *manager, Group *toDelete);

/**
 * @brief Prints all valid (non-empty) groups managed by the GroupManager.
 *
 * Iterates through the group array and prints each initialized group using `printGroup()`.
 *
 * @param manager Pointer to the GroupManager.
 * @return 1 on success, -1 if manager is NULL or no groups to print.
 *
 * @see printGroup
 */
int printGroupManager(GroupManager *manager);

/**
 * @brief Frees all memory associated with the GroupManager and its groups.
 *
 * Calls `deleteGroup()` on each individual group before freeing the array and manager itself.
 *
 * @param manager Pointer to the GroupManager to delete. Safe to call with NULL.
 *
 * @see deleteGroup
 */
void deleteGroupManager(GroupManager *manager);

#endif // GROUP_MANAGER_H

