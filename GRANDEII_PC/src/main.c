#include "view/view.h"
#include "arduino/communication.h"
#include "controller/live_controller.h"
#include "model/group_manager.h"
#include "model/json_parser.h"

int main(int argc, char** argv){

    if (GLIB_MAJOR_VERSION!=2 || GLIB_MINOR_VERSION!=80){
        fprintf(stderr, "ATTENTION LE PROJET A ETE CODE AVEC GLIB 2.80!\n");
        fprintf(stderr, "Vous avez actuellement GLIB%d.%d, merci de mettre à jour GLIB pour le bon fonctionnement du programme\n", GLIB_MAJOR_VERSION, GLIB_MINOR_VERSION);
    }

    //Create Live controller
    global_live_controller = createLiveController();


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
    funct1->ir_code[0] = 0x11;
    funct1->ir_code[1] = 0x12;
    funct1->ir_code[2] = 0x13;
    funct1->ir_code[3] = 0x14;

    print_function_info(funct1);
    insertFirst(light1->function_list, funct1, LIST_TYPE_FUNCTION,compareFunction);
    printList(light1->function_list, print_function_info);

    addLighttoGroup(Group1, light1);
    addLighttoGroup(Group1, light2);

    assignGroupToManager(global_live_controller->manager, Group1);

    //======= GROUP 2: MANUAL ============
    Group *group2 = allocGroup();
    initGroup(group2, "Groupe Manuel 2", 70, blue);

    Light *lightC = alloc_light();
    initNewLight(lightC, "Projecteur Bleu", INFRAROUGE);
    setColor(lightC, blue); setBrightness(lightC, 180);
    addLighttoGroup(group2, lightC);

    assignGroupToManager(global_live_controller->manager, group2);
    printf("Groupes importé manuellement :\n");
    for (int i=0; i<3; i++){
        printGroup(global_live_controller->manager->groups[i]);
    }

    //========== GROUP 3: IMPORTED FROM JSON ========

    int nb_groups_parsed=0;
    Group** groups_parsed = parse_device_file(DATA_PATH "/data.json", &nb_groups_parsed);
    if (groups_parsed!=NULL) {
        printf("Groupe importé depuis le fichier JSON :\n");
        for (int i=0; i<nb_groups_parsed; i++){
            printGroup(groups_parsed[i]);
        }

    } else {
        fprintf(stderr, "Erreur : Impossible d'importer le groupe depuis %s\n", DATA_PATH);
    }
    assignGroupToManager(global_live_controller->manager, groups_parsed[0]);
    assignGroupToManager(global_live_controller->manager, groups_parsed[1]);
    assignGroupToManager(global_live_controller->manager, groups_parsed[2]);

    global_live_controller->arduino_controller->port = "/dev/ttyACM0";
    if (open_port(global_live_controller->arduino_controller)==-1){
        fprintf(stderr, "Error opening arduino controller\n");
    }
        ///VIEW

    main_view(argc, argv);
    write_device_file(global_live_controller->manager->groups, 3, DATA_PATH "/data.json");
    //========== CLEAN UP ===========
    free_color(test);

    //The live controller has all the color, lights, function, ...
    deleteLiveController(global_live_controller);
}
