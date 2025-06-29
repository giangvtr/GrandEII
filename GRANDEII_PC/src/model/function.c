//
// Created by boris on 4/27/25.
//

#include "function.h"
#include <stdlib.h>


Function* alloc_function() {
    Function* this_function = malloc(sizeof(Function));

    if (this_function == NULL) {
        fprintf(stderr, "Memory allocation failed for color\n");
        return NULL;  // Or handle it according to your needs
    }
    return this_function;
}

int set_funct(Function * this_function, FunctionName name){
    if (this_function == NULL) {
        fprintf(stderr, "Function not yet allocated\n");
        return -1;
    }
    this_function->name = name;

    return 0;
}

void print_function_info(Function* function){
  if (function != NULL) {
        printf("\t\tFunction name: %d\n", function->name);
    } else {
        fprintf(stderr, "Error: Invalid function object\n");
    }
}

int compareFunction(const void* a, const void* b) {
    Function* functA = (Function*)a;
    Function* functB = (Function*)b;
    if (functA == NULL || functB == NULL) return 0;
    if (functA->name == functB->name) return 1;
    return 0;
}

void free_function(void* data) {
    if (data != NULL) {
        free(data);
    }
}