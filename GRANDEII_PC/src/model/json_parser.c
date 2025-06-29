//
// Created by giangvu on 4/6/25.
//

#include "json_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "lib/cJSON/cJSON.h"
#include <string.h>

//TODO all the checks

static int parse_colors_JSON(cJSON* colorsArrayJSON, int nbColors, List* colors){
    for (int colorIndex = 0; colorIndex < nbColors; colorIndex++){
        cJSON* colorJSON = cJSON_GetArrayItem(colorsArrayJSON, colorIndex);

        int r = cJSON_GetObjectItem(colorJSON, "r")->valueint;
        int g = cJSON_GetObjectItem(colorJSON, "g")->valueint;
        int b = cJSON_GetObjectItem(colorJSON, "b")->valueint;
        u_char ir_code[4];
        INT_to_IR_Code(cJSON_GetObjectItem(colorJSON, "IR Code")->valueint, ir_code);

        Color* color = alloc_color();
        set_color(color, r, g, b);
        memcpy(color->ir_code, ir_code,4);

        insertFirst(colors, color, LIST_TYPE_COLOR, compareColorExact);
    }
    return 0;
}

static int parse_function_JSON(cJSON* functionsArrayJSON, int nbFunctions, List* functions){
    for (int functionIndex = 0; functionIndex < nbFunctions; functionIndex++){
        cJSON* functionJSON = cJSON_GetArrayItem(functionsArrayJSON, functionIndex);
        Function* function = alloc_function();

        int function_num = cJSON_GetObjectItem(functionJSON, "Function")->valueint;
        function->name = function_num;
        u_char ir_code[4];
        cJSON* test =  cJSON_GetObjectItem(functionJSON, "IR Code");
        int valueIR =cJSON_GetNumberValue(test);
        INT_to_IR_Code(valueIR, ir_code);
        memcpy(function->ir_code, ir_code,4);

        insertFirst(functions, function, LIST_TYPE_FUNCTION, compareFunction);
    }
    return 0;
}

static int parse_light_JSON(cJSON* lightArrayJSON, int nbLights, Group* group){
    for (int lightIndex = 0;lightIndex < nbLights; lightIndex++){
        //Get light
        cJSON* lightJSON = cJSON_GetArrayItem(lightArrayJSON, lightIndex);

        char* lightName = cJSON_GetObjectItem(lightJSON, "name")->valuestring;
        int lightTech = cJSON_GetObjectItem(lightJSON, "technology")->valueint;

        Light *light = alloc_light();
        initNewLight(light, lightName, lightTech);

        //Parse colors
        int lightNbColors = cJSON_GetObjectItem(lightJSON, "nbCouleurs")->valueint;
        if (lightNbColors > 0){
            cJSON* colorArrayJSON = cJSON_GetObjectItem(lightJSON, "Couleurs");
            parse_colors_JSON(colorArrayJSON, lightNbColors, light->color_list);
        }
        //Parse function
        int lightNbFunction = cJSON_GetObjectItem(lightJSON, "nbFonctions")->valueint;
        if (lightNbFunction > 0){
            cJSON* functionArrayJSON = cJSON_GetObjectItem(lightJSON, "functs");
            parse_function_JSON(functionArrayJSON, lightNbFunction, light->function_list);
        }
        printList(light->function_list, print_function_info);
        addLighttoGroup(group, light);

    }

    return 0;
}

Group **parse_device_file(const char *filename, int* nbGroups){
    // Écriture dans le fichier
    FILE *file = fopen(filename, "r");
    fseek(file, 0,SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (file) {
        fread(buffer, size, 1, file);
        fclose(file);
    } else {
        perror("Erreur d’écriture du fichier");
    }

    cJSON *parsedJSON = cJSON_Parse(buffer);
    if (parsedJSON == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) fprintf(stderr, "Error before: %s\n", error_ptr);
        return NULL;
    }

    //This is where the fun begins
    Color* default_color = alloc_color();
    set_color(default_color, 255, 0, 0);

    int nbParsedGroups = cJSON_GetObjectItemCaseSensitive(parsedJSON, "nbGroups")->valueint;
    if (nbParsedGroups <= 0){
        fprintf(stderr, "Cannot parse JSON Correctly, missing groups");
        *nbGroups = -1;
        return NULL;
    }
    *nbGroups = nbParsedGroups;

    Group** returnGroup = (Group**)malloc(sizeof(Group*)*nbParsedGroups);

    //Cycle through each groups
    for (int groupIndex = 0;groupIndex < nbParsedGroups; groupIndex++){


        //get current
        char string_index[2];//No more than 99 groups I hope
        sprintf(string_index, "%d", groupIndex);
        cJSON *groupJSON = cJSON_GetObjectItemCaseSensitive(parsedJSON, string_index);

        //Alloc group
        Group* group = allocGroup();
        initGroup(group, cJSON_GetObjectItem(groupJSON, "Name")->valuestring, 100, default_color);

        //Get light info and nb
        int nbLights = cJSON_GetObjectItem(groupJSON, "nbLights")->valueint;
        cJSON* lightArray = cJSON_GetObjectItem(groupJSON, "Lights");
        parse_light_JSON(lightArray, nbLights, group);




        returnGroup[groupIndex] = group;
        //Free some stuff
        //cJSON_Delete(groupJSON);
        //cJSON_Delete(lightArray);
    }
    //Delete all
    cJSON_Delete(parsedJSON);
    free(buffer);
    return returnGroup;
}

int valider_device(const int *device) {
    return 0;
    // if (strlen(device->machine.name) == 0 || strlen(device->machine.name) > 50)
    //     return 0;
    //
    // if (device->machine.light_settings.intensity < 0 || device->machine.light_settings.intensity > 100)
    //     return 0;
    //
    // if (device->machine.light_settings.speed <= 0)
    //     return 0;
    //
    // const char *color = device->machine.light_settings.color;
    // if (strlen(color) != 7 || color[0] != '#')
    //     return 0;
    //
    // // Vérifie chaque caractère hexadécimal
    // for (int i = 1; i < 7; i++) {
    //     if (!((color[i] >= '0' && color[i] <= '9') ||
    //           (color[i] >= 'A' && color[i] <= 'F') ||
    //           (color[i] >= 'a' && color[i] <= 'f')))
    //         return 0;
    // }
    //
    // // Vérifie status
    // const char *status = device->machine.status.arduino_status;
    // if (strcmp(status, "connecté") != 0 && strcmp(status, "déconnecté") != 0)
    //     return 0;
    //
    // return 1;
}
