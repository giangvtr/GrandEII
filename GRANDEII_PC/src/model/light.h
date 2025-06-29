
/**
 * @file light.h
 * @brief Header file for controlling lights with customizable properties.
 *
 * This file contains the definition of a `Light` structure and functions
 * to manage its properties, such as setting brightness, color, and port.
 */

#ifndef LIGHT_H
#define LIGHT_H

#define MAX_NAME_LENGTH 50

#include "color.h"
#include "function.h"
#include "list_common.h"

/**
 * @enum technologyLight
 * @brief Enumerates the technology types a light can use.
 */
typedef enum {
    NO_TECH_CHOSEN,   /**< No technology chosen */
    INFRAROUGE        /**< Infrared technology */
} technologyLight;

/**
 * @brief Represents a light with customizable properties.
 */
typedef struct {
    char name[MAX_NAME_LENGTH];         /**< Name of the light */
    int brightness;                     /**< Brightness level (0-255) */
    Color* color;                       /**< Current color*/
    List *color_list;                   /**< Pointer to all the color possible*/
    List *function_list;                   /**< Pointer to all the funtion possible*/
    technologyLight technology;         /**< The technology of the light (e.g., infrarouge, bluetooth, Wi-fi, etc.) */
    uint8_t id;
} Light;

/**
 * @brief Allocate a Light structure.
 * @return Pointer to the allocated Light, or NULL on error.
 */
Light* alloc_light(void);

/**
 * @brief Initialize a Light structure with specified properties.
 * @param light Pointer to an allocated Light structure.
 * @param name The name of the light.
 * @param technology The technology type of the light.
 * @param port The port on Arduino where the light is connected.
 * @return 0 if successful, -1 if error
 */
int initNewLight(Light* light, const char* name, technologyLight technology);

/**
 * @brief Sets the color of the specified light.
 *
 * This function updates the color property of the given light.
 *
 * @param light A pointer to the Light structure.
 * @param color A pointer to the new Color structure for the light.
 * @return 0 if successful, -1 if error
 */
int setColor(Light* light, Color* color);

/**
 * @brief Sets the brightness of the specified light.
 *
 * This function updates the brightness level of the given light.
 *
 * @param light A pointer to the Light structure.
 * @param brightness The new brightness level (0-255).
 * @return 0 if successful, -1 if error
 *
 */
int setBrightness(Light* light, int brightness);

/**
 * @brief Sets the name of the specified light.
 *
 * This function updates the name of the given light.
 *
 * @param light A pointer to the Light structure.
 * @param name The new name array.
 * @return 0 if successful, -1 if error
 *
 */
int setLightName(Light* light, const char* name);



/**
 * @brief Return light ID
 * @param light A pointer to the Light structure.
 * @return the id of the light, -1 if error.
 */
uint32_t getLightId(const Light* light);

/**
 * @brief Return light's name
 * @param light A pointer to the Light structure.
 * @return light->name, NULL if error.
 */
const char* getLightName(const Light* light);

/**
 * @brief Print the details of the specified light.
 * @param light Pointer to the Light structure.
 * @return 0 on success, -1 on error.
 */
int printLight(Light* light);

/**
 * @brief Deletes the specified light and frees associated resources.
 * This function deletes a light object and frees any dynamically allocated memory.
 * @param data A pointer to the Light structure to be deleted.
 */
void deleteLight(void* data);

/**
 * @brief Compare if two objects are the same light
 * The function cast void* into type Light* and compare their id (since the id is unique)
 * @param a pointer to the structure to be compared.
 * @param b pointer to the structure to be compared.
 * @return 0 if not the same light, 1 if they are the same light
 */
int compareLight( void* a,  void* b);

/**
 * @brief Compare all the color in the light and return the nearest color relative to the one in params
 * @param light The active light
 * @param color_to_compare the color to compare
 * @return the Color found in the light
 */
Color* getNearestColor(Light* light, Color* color_to_compare);

/**
 * @brief Return the function present in the light
 * @param light the active light
 * @param function_to_compare the function to be retrieved
 * @return the function found in the light
 */
Function* getFunction(Light* light, FunctionName function_to_compare);

/**
 * @brief Add a color to the light's color list.
 * @param light Pointer to the Light structure.
 * @param color Pointer to the Color to add.
 * @return 0 if successful, -1 on error.
 */
int add_color_to_light(Light* light, Color* color);

/**
 * @brief Add a function to the light's function list.
 * @param light Pointer to the Light structure.
 * @param function_to_add Pointer to the Function to add.
 * @return 0 if successful, -1 on error.
 */
int add_function_to_light(Light* light, Function* function_to_add);

#endif // LIGHT_H



