//
// Created by boris on 4/27/25.
//

/**
 * @file function.c
 * @brief Implémentation des fonctions pour la gestion des structures Function.
 *
 * Ce fichier fournit des fonctions pour allouer, initialiser, afficher,
 * comparer et libérer des objets de type `Function`, qui représentent
 * probablement des opérations ou types de transformation dans un système donné.
 */

#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdint.h>  // For uint8_t and uint32_t
#include <stdlib.h>
#include "../arduino/communication.h"



/**
* @enum FunctionName
* @brief Enumerates all possible functions for a light.
* This enum defines the possible operational modes and actions
* that can be assigned to a light's function.
*/
typedef enum {
    OFF,
    ON,
    FLASH,
    FADE1,
    FADE2,
    FADE3,
    AUTO1,
    AUTO2,
    AUTO3,
    BRIGHTNESS_MORE,
    BRIGHTNESS_LESS,
    STROBE,
    JUMP,
    WHITE_MODE
} FunctionName;

/**
 * @struct Function
 * @brief Represents function of the light, like on, off or anything else
 */
typedef struct {
    FunctionName name;
    unsigned char ir_code[IR_CODE_LENGTH]; /**<Unique IR code for function */
} Function;

/**
* @brief Allocate a Function structure.
* @return the pointer to the allocated sturcture
*/
Function* alloc_function();

/**
 * @brief set a Function structure
 *
 * @param name name of the fonction
 * @param this_function the pointer to the color already allocated
 * @return 0 if successful, error if any other else
 */
int set_funct(Function * this_function, FunctionName name);

/**
 * @brief Prints the Function information
 *
 * @param function The Function structure to display.
 */
void print_function_info(Function* function);

/**
 * @brief Compare two functions
 * @param a function 1
 * @param b function 2
 * @return 1 if the function are the same, 0 else
 */
int compareFunction(const void* a, const void* b);

/**
 * @brief Free the Function structure
 * @param data the function structure to be freed
 */
void free_function(void* data);

#endif //FUNCTION_H
