//
// Created by maxime on 05/05/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/cJSON/cJSON.h"
#include "json_parser.h"
#include "function.h"
#include "group.h"


/**
typedef struct{
    char *name;
    List *lights;
    int nbLights;
    int group_brightness;
    Color *group_color;
    uint8_t id;
} Group;
**/

void write_device_file(const Group *group_list[], int nb_groups, const char *filename) {
    // Création de l'objet JSON racine
    cJSON *root = cJSON_CreateObject();

    cJSON_AddNumberToObject(root, "nbGroups", nb_groups);

    for (int curr_group_index = 0; curr_group_index < nb_groups; curr_group_index++){
        const Group *group = group_list[curr_group_index];
        cJSON *group_root= cJSON_CreateObject();
        // Ajout nom
        cJSON_AddStringToObject(group_root, "name", group->name);
        cJSON_AddNumberToObject(group_root, "nbLights", group->nbLights);

        //Liste lumières
        cJSON *lightArray = cJSON_CreateArray();
        setOnFirst(group->lights);
        for (int i=0; i<group->nbLights; i++){
            cJSON *light = cJSON_CreateObject();
            Light* currentLight = group->lights->current->data;

            int nb_couleurs = nbElement(currentLight->color_list);
            int nb_fonctions = nbElement(currentLight->function_list);

            //Ajout nom et tech des lumières
            cJSON_AddStringToObject(light, "name", currentLight->name);
            cJSON_AddNumberToObject(light, "technology", currentLight->technology);
            cJSON_AddNumberToObject(light, "nbCouleurs", nb_couleurs);
            cJSON_AddNumberToObject(light, "nbFonctions", nb_fonctions);

            //Ajout d'une lste de toutes les couleurs
            cJSON *colorArray = cJSON_CreateArray();
            cJSON_AddItemToObject(light, "Couleurs", colorArray);
            setOnFirst(currentLight->color_list);
            for (int j=0;j<nb_couleurs; j++){
                cJSON *color = cJSON_CreateObject();
                Color* currentColor = currentLight->color_list->current->data;
                //Ajout rgb des couleurs ainsi que leur codes rgb
                cJSON_AddNumberToObject(color, "r", currentColor->r);
                cJSON_AddNumberToObject(color, "g", currentColor->g);
                cJSON_AddNumberToObject(color, "b", currentColor->b);
                cJSON_AddNumberToObject(color, "IR Code" , IR_Code_to_INT(currentColor->ir_code));

                cJSON_AddItemToArray(colorArray, color);
                setOnNext(currentLight->color_list);
            }

            cJSON *functArray = cJSON_CreateArray();
            cJSON_AddItemToObject(light, "functs", functArray);
            setOnFirst(currentLight->function_list);
            for (int j=0;j<nb_fonctions; j++){
                cJSON *funct = cJSON_CreateObject();
                Function* currentFunct = currentLight->function_list->current->data;
                cJSON_AddNumberToObject(funct, "Function", currentFunct->name);
                cJSON_AddNumberToObject(funct, "IR Code" , IR_Code_to_INT(currentFunct->ir_code));

                cJSON_AddItemToArray(functArray, funct);
                setOnNext(currentLight->function_list);
            }
            //Ajout hierarchique
            //cJSON_AddItemToArray(light, functArray);
            cJSON_AddItemToArray(lightArray, light);
            setOnNext(group->lights);
        }

        //Adding the lights array to the group
        cJSON_AddItemToObject(group_root, "Lights", lightArray);

        char string_index[2];//No more than 99 groups I hope
        sprintf(string_index, "%d", curr_group_index);
        cJSON_AddItemToObject(root, string_index, group_root );
    }

    // Conversion de l'objet JSON en chaîne de caractères
    char *json_str = cJSON_Print(root);

    // Écriture dans le fichier
    FILE *file = fopen(filename, "w+");
    if (file) {
        fputs(json_str, file);
        fclose(file);
    } else {
        perror("Erreur d’écriture du fichier");
    }

    // Libération de la mémoire
    cJSON_Delete(root);
    free(json_str);
}