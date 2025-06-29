#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "function.h"
#include "group.h"
#include "group_manager.h"
#include "json_parser.h"
#include "light.h"

int main() {
    printf("Running main_console test...\n");
    //===== CREATE A GROUP MANAGER =====
    GroupManager * myListOfGroups = createGroupManager(3);


    //===== CREATE PREDEFINED COLOR PALETTE =====
    Color* red = alloc_color(); set_color(red, 255, 0, 0);
    Color* green = alloc_color(); set_color(green, 0, 255, 0);
    Color* blue = alloc_color(); set_color(blue, 0, 0, 255);
    Color* yellow = alloc_color(); set_color(yellow, 255, 255, 0);
    Color* cyan = alloc_color(); set_color(cyan, 0, 255, 255);
    Color* magenta = alloc_color(); set_color(magenta, 255, 0, 255);
    Color* orange = alloc_color(); set_color(orange, 255, 165, 0);
    Color* purple = alloc_color(); set_color(purple, 128, 0, 128);
    Color* pink = alloc_color(); set_color(pink, 255, 192, 203);
    Color* lime = alloc_color(); set_color(lime, 191, 255, 0);

 print_color_info(red);

//===== GROUP 1: MANUAL ======
 Group* Group1 = allocGroup();
 initGroup(Group1, "Mon super groupe de lumières", 100, cyan);

 // Create and configure Light 1
 Light* light1 = alloc_light();
initNewLight(light1, "Light 1", INFRAROUGE);

 insertFirst(light1->color_list, red, LIST_TYPE_COLOR, compareColorExact);
 insertFirst(light1->color_list, green, LIST_TYPE_COLOR, compareColorExact);
 insertFirst(light1->color_list, blue, LIST_TYPE_COLOR,compareColorExact);
 insertFirst(light1->color_list, yellow, LIST_TYPE_COLOR,compareColorExact);
 insertFirst(light1->color_list, cyan, LIST_TYPE_COLOR,compareColorExact);
 insertFirst(light1->color_list, magenta, LIST_TYPE_COLOR,compareColorExact);
 insertFirst(light1->color_list, orange, LIST_TYPE_COLOR,compareColorExact);
 insertFirst(light1->color_list, purple, LIST_TYPE_COLOR, compareColorExact);
 insertFirst(light1->color_list, pink, LIST_TYPE_COLOR, compareColorExact);
 insertFirst(light1->color_list, lime, LIST_TYPE_COLOR,compareColorExact);

 printList(light1->color_list, print_color_info);
 printf("%d\n", nbElement(light1->color_list));
 printf("Nearest color\n");
 Color* test = alloc_color();
 set_color(test, 47, 97, 170);
 Color* result = getNearestColor(light1, test);
 print_color_info(result);
 // Change the color of light1 to red and set brightness to 150
 setColor(light1, red);
 setBrightness(light1, 150);

 // Print the updated details of light1
 printf("\nLight 1 details (after changes to color and brightness):\n");
 printLight(light1);

 // Create and configure Light 2 with a specific color (green)
 Light* light2 = alloc_light();
 initNewLight(light2,"Light 2", INFRAROUGE);
 setColor(light2, red);
 setBrightness(light2, 200);

 // Print the details of light2
 printf("\nLight 2 details (green color):\n");
 printLight(light2);


 Function* funct1 = alloc_function();
 set_funct(funct1, ON);
 print_function_info(funct1);
 insertFirst(light1->function_list, funct1, LIST_TYPE_FUNCTION,compareFunction);
 printList(light1->function_list, print_function_info);

addLighttoGroup(Group1, light1);
addLighttoGroup(Group1, light2);

assignGroupToManager(myListOfGroups, Group1);

 //======= GROUP 2: MANUAL ============
 Group *group2 = allocGroup();
 initGroup(group2, "Groupe Manuel 2", 70, blue);

 Light *lightC = alloc_light();
 initNewLight(lightC, "Projecteur Bleu", INFRAROUGE);
 setColor(lightC, blue); setBrightness(lightC, 180);
 addLighttoGroup(group2, lightC);

 assignGroupToManager(myListOfGroups, group2);

 //========== GROUP 3: IMPORTED FROM JSON ========
 /*
 Group *group3 = parse_device_file(DATA_PATH);
 if (group3) {
   printf("Groupe importé depuis le fichier JSON :\n");
   printGroup(group3);
   assignGroupToManager(myListOfGroups, group3);
 } else {
  fprintf(stderr, "Erreur : Impossible d'importer le groupe depuis %s\n", DATA_PATH);
 }
 */


 //========== CLEAN UP ===========
 //free_function(funct1);
 deleteLight(light1);
 deleteLight(light2);
 deleteLight(lightC);
 free_color(red);
 free_color(green);
 free_color(blue);
 free_color(yellow);
 free_color(cyan);
 free_color(magenta);
 free_color(orange);
 free_color(purple);
 free_color(pink);
 free_color(lime);

 deleteGroupManager(myListOfGroups);
 return 0;
}
