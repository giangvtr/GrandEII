//
// Created by giangvu on 4/21/25.
//

#include "group_manager.h"

// user choose to make how many empty groups
GroupManager *createGroupManager(int numGroups) {
    if (numGroups <= 1 || numGroups > MAX_GROUP) {
        fprintf(stderr, "Number of groups must be between 1 and %d\n", MAX_GROUP);
        return NULL;
    }

    //Allocate a group manager
    GroupManager *manager = malloc(sizeof(GroupManager));
    if (!manager) return NULL; //if allocation fails

    // Allocate memory for the groups array
    manager->groups = (Group **)malloc(numGroups * sizeof(Group*));
    if (manager->groups == NULL) {
        fprintf(stderr, "Memory allocation failed for groups array\n");
        free(manager);
        return NULL;
    }

    for (int i = 0; i < numGroups; i++) {
        char groupName[20];
        sprintf(groupName, "Group %d", i + 1);

        // Initialize each group (assuming Group has an initialization function)
        Group *newGroup = allocGroup();
        if (initGroup(newGroup, groupName, 0, NULL) == 0) manager->groups[i] = newGroup;


        //If allocate group fails
        if (!manager->groups[i] || initGroup(manager->groups[i], groupName, 0, NULL) != 0) {
            // Cleanup previously allocated groups
            for (int j = 0; j < i; j++) {
                deleteGroup(manager->groups[j]);
                free(manager->groups[j]);
            }
            free(manager->groups);
            free(manager);
            return NULL;
        }
    }
    manager->numGroups = numGroups;
    return manager;
}

int addEmptyGroup(GroupManager *manager) {
    if (manager == NULL) {
        fprintf(stderr, "NULL pointer to manager\n");
        return -1;
    }

    if (manager->numGroups >= MAX_GROUP) {
        fprintf(stderr, "Maximum number of groups reached, cannot add more\n");
        return -1;
    }

    char groupName[20];
    sprintf(groupName, "Group %d", manager->numGroups + 1);

    Group *newGroup = allocGroup();
    if (initGroup(newGroup, groupName, 0, NULL) == 0) manager->groups[manager->numGroups] = newGroup;
    if (newGroup == NULL) {
        fprintf(stderr, "Failed to allocate new group\n");
        return -1;
    }

    if (initGroup(newGroup, groupName, 0, NULL) != 0) {
        fprintf(stderr, "Failed to initialize new group\n");
        free(newGroup);
        return -1;
    }

    manager->groups[manager->numGroups] = newGroup;
    manager->numGroups++;

    return 1;
}

int assignGroupToManager(GroupManager *manager, Group *group) {
    if (manager == NULL || group == NULL) {
        fprintf(stderr, "NULL pointer in assignGroupToManager\n");
        return -1;
    }

    for (int i = 0; i < manager->numGroups; i++) {
        if (isEmptyGroup(manager->groups[i])) {
            //Delete empty group
            deleteGroup(manager->groups[i]);

            // Copy the entire group struct
            manager->groups[i] = group;

            // Rename the group safely
            char newName[20];
            sprintf(newName, "Group %d", i + 1);

            free(manager->groups[i]->name); // Free up the old name
            manager->groups[i]->name = (char *)malloc(strlen(newName) + 1);
            if (manager->groups[i]->name == NULL) {
                fprintf(stderr, "Memory allocation failed for new group name\n");
                return -1;
            }
            strcpy(manager->groups[i]->name, newName);

            return 1; // Success
        }
    }

    fprintf(stderr, "No empty group slot available in manager\n");
    return -1; // No available slot
}

int removeGroup(GroupManager *manager, Group* toDelete) {
    if (manager == NULL || toDelete == NULL) {
        fprintf(stderr, "NULL pointer of manager or group\n");
        return -1;
    }

    // Find the group to delete
    for (int i = 0; i < manager->numGroups; i++) {
        printf("Start search matching group \n");
        if (compareGroup(toDelete, manager->groups[i]) == 1) {
            // Free the group
            printf("Group found \n");
            deleteGroup(manager->groups[i]);

            // Shift subsequent groups to the left
            for (int j = i + 1; j < manager->numGroups; j++) {
                manager->groups[j - 1] = manager->groups[j];

                // Update group name based on new position
                char newName[20];
                sprintf(newName, "Group %d", j);  // j because it moves to index j-1

                // Reallocate and copy new name
                manager->groups[j - 1]->name = realloc(manager->groups[j - 1]->name, strlen(newName) + 1);
                if (manager->groups[j - 1]->name != NULL) {
                    strcpy(manager->groups[j - 1]->name, newName);
                } else {
                    fprintf(stderr, "Memory allocation failed when renaming group\n");
                    return -1;
                }
            }
            manager->numGroups--;
            return 1;  // Success
        }
    }

    printf("Group not found\n");
    return -1;
}

int printGroupManager(GroupManager *manager) {
    if (manager == NULL || manager->numGroups <= 0) {
        fprintf(stderr, "No groups to print\n");
        return -1;
    }
    int index = 0;

    while (index < manager->numGroups && !isEmptyGroup(manager->groups[index])) {
        printGroup(manager->groups[index]);
        index++;
    }
    return 1;
}

void deleteGroupManager(GroupManager *manager) {
    if (!manager) return;

    for (int i = 0; i < manager->numGroups; ++i) {
        deleteGroup(manager->groups[i]);
    }
    free(manager->groups);
    free(manager);
}
