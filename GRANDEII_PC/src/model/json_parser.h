//
// Created by giangvu on 4/6/25.
//

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "lib/cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include "group.h"
#include "../arduino/arduino_utils.h"
Group **parse_device_file(const char *filename, int* nbGroups);
void write_device_file(const Group *group_list[], int nb_groups, const char *filename);
//int valider_device(const Device *device);



#endif //JSON_PARSER_H
