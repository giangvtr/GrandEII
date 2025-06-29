//
// Created by giangvu on 3/13/25.
//

#include "group.h"
#include "color.h"


static uint8_t last_generated_id = 0;

Group* allocGroup() {
    Group* group = NULL;
    group = (Group*)malloc(sizeof(Group));
    if (group == NULL) {
        fprintf(stderr, "Error allocate group\n");
        return NULL;
    }
    group->name = NULL;
    group->lights = NULL;
    group->nbLights = 0;
    group->group_brightness = 0;
    group->group_color = NULL;
    group->id = 0;
    return group;
}

int initGroup(Group *group, const char *name, int group_brightness, Color *group_color) {
    if(group==NULL||name == NULL){
      return -1;
    }

    free(group->name);//Free the old name before realloc
    group->name = malloc(strlen(name)+1);   /**< Allocate memory for the group name */
    if (group->name == NULL) return -1;
    strcpy(group->name, name);                /**< Copy the name into the allocated memory */
    group->id = ++last_generated_id;          /**< Assign an incremented ID */
    group->group_brightness = group_brightness; /**< Set the initial brightness */
    group->group_color = group_color;         /**< Set the initial color */

    deleteList(group->lights, deleteLight); // Free the old light
    group->lights = allocList();
    initList(group->lights, LIST_TYPE_LIGHT);  /**< Initialize the list of lights */
    group->nbLights = 0;
    return 0;
}

int addLighttoGroup(Group *group, Light *light) {
    if (group == NULL || light == NULL) {
        fprintf(stderr, "Pointer NULL\n");
        return -1;
    }
    if (group->nbLights >= MAX_LIGHTS) {
        fprintf(stderr, "Too many lights in the group\n");
        return -1;
    }
    if (insertFirst(group->lights, light, LIST_TYPE_LIGHT, compareLight) == 1) {
        group->nbLights++;
        return 0;  // success
    }
    return -1;  // failure
}


int isEmptyGroup(const Group *group) {
    if(group==NULL ) {
        fprintf(stderr,"Pointer NULL\n");
        return -1;
    }
    if (isEmpty(group->lights) == 1|| group->nbLights == 0 || group->lights == NULL) {
        return 1;
    }
    return 0;
}

void setGroupBrightness(Group *group, int brightness) {
  if(group !=NULL)  group->group_brightness = brightness;  /**< Set the group brightness */
}

void setGroupColor(Group *group, Color *color) {
    if(group != NULL) group->group_color = color;  /**< Set the group color */
}

const char *getGroupName(const Group *group) {
    if(group != NULL) return group->name;  /**< Return the group name */
    return NULL;
}

uint8_t getGroupID(const Group *group) {
  if(group != NULL) return group->id;
  return 0;
}

Color *getGroupColor(const Group *group){
  if(group != NULL) return group->group_color;
  return NULL;
}

int compareGroup(const Group * groupA, const Group* groupB) {
    if (groupA == groupB) return 1;
    if (!groupA || !groupB) return 0;

    // Compare by ID first (faster and guaranteed unique)
    if (groupA->id == groupB->id) return 1;

    // Fallback to name comparison if IDs don't match
    if (groupA->name && groupB->name) {
        return strcmp(groupA->name, groupB->name) == 0;
    }

    return 0;
}

int printGroup(Group *group) {
    if (group == NULL) {
        printf("Group is NULL.\n");
        return -1;
    }

    printf("=== Group Information ===\n");

    printf("Name         : %s\n", group->name ? group->name : "Unnamed");
    printf("ID           : %u\n", group->id);
    printf("Brightness   : %d\n", group->group_brightness);

    if (group->group_color) {
        printf("Color        : (R: %d, G: %d, B: %d)\n",
               group->group_color->r,
               group->group_color->g,
               group->group_color->b);
    } else {
        printf("Color        : None\n");
    }

    printf("Number Lights: %d\n", group->nbLights);

    if (group->lights == NULL || group->nbLights == 0) {
        printf("Lights       : No lights in this group.\n");
    } else {
        printf("Lights       :\n");
        printList(group->lights,printLight);

    }
    printf("==========================\n");
    return 0;
}

void deleteGroup(Group *group) {
    if(group != NULL) {
        if (group->name) {
            free(group->name);  /**< Free the allocated memory for the group name */
            //printf("Free group name successfully \n");
        }
        if (group->lights) {
            deleteList(group->lights, deleteLight);  /**< Free all lights in the list */
            //group->lights = NULL;
        }
        group->group_brightness = 0; /**< Reset the group brightness */
        group->group_color = NULL; /**< Set the group color to NULL */
        free(group);
        //group = NULL;
    }
}