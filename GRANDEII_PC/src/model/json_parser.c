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
    *nbGroups = -1;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur de lecture du fichier");
        return NULL;
    }
    fseek(file, 0,SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }
    size_t nbRead = fread(buffer, 1, size, file);
    buffer[nbRead] = '\0';
    fclose(file);

    cJSON *parsedJSON = cJSON_Parse(buffer);
    free(buffer);
    if (parsedJSON == NULL){
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) fprintf(stderr, "Error before: %s\n", error_ptr);
        return NULL;
    }

    //Shared by every parsed group, so it must outlive them: static, not heap
    static Color default_color;
    set_color(&default_color, 255, 0, 0);

    cJSON *nbGroupsJSON = cJSON_GetObjectItemCaseSensitive(parsedJSON, "nbGroups");
    if (!cJSON_IsNumber(nbGroupsJSON) || nbGroupsJSON->valueint <= 0){
        fprintf(stderr, "Cannot parse JSON Correctly, missing groups\n");
        cJSON_Delete(parsedJSON);
        return NULL;
    }
    int nbParsedGroups = nbGroupsJSON->valueint;

    Group** returnGroup = (Group**)calloc(nbParsedGroups, sizeof(Group*));
    if (returnGroup == NULL) {
        cJSON_Delete(parsedJSON);
        return NULL;
    }

    //Cycle through each groups
    for (int groupIndex = 0;groupIndex < nbParsedGroups; groupIndex++){
        //get current
        char string_index[12];
        sprintf(string_index, "%d", groupIndex);
        cJSON *groupJSON = cJSON_GetObjectItemCaseSensitive(parsedJSON, string_index);
        cJSON *nameJSON = cJSON_GetObjectItem(groupJSON, "Name");
        cJSON *nbLightsJSON = cJSON_GetObjectItem(groupJSON, "nbLights");
        cJSON *lightArray = cJSON_GetObjectItem(groupJSON, "Lights");

        Group* group = allocGroup();
        if (!cJSON_IsString(nameJSON) || !cJSON_IsNumber(nbLightsJSON) || !cJSON_IsArray(lightArray)
            || group == NULL || initGroup(group, nameJSON->valuestring, 100, &default_color) != 0) {
            fprintf(stderr, "Cannot parse group %d\n", groupIndex);
            deleteGroup(group);
            for (int i = 0; i < groupIndex; i++) deleteGroup(returnGroup[i]);
            free(returnGroup);
            cJSON_Delete(parsedJSON);
            return NULL;
        }

        parse_light_JSON(lightArray, nbLightsJSON->valueint, group);
        returnGroup[groupIndex] = group;
    }
    *nbGroups = nbParsedGroups;
    cJSON_Delete(parsedJSON);
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
