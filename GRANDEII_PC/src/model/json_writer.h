//
// Created by maxime on 30/05/25.
//

#ifndef JSON_WRITER_H
#define JSON_WRITER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/cJSON/cJSON.h"
#include "json_parser.h"
#include "function.h"
#include "group.h"


void write_device_file(const Group *group_list[], int nb_groups, const char *filename);

#endif //JSON_WRITER_H
