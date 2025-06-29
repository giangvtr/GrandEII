/**
*@file light_wizard.h
*@author Boris
*@brief This is the setup of the lights, using arduino and everything
*/

#ifndef LIGHT_WIZARD_H
#define LIGHT_WIZARD_H

#include "../arduino/communication.h"
#include "../model/color.h"
#include "../model/function.h"
#include <gtk/gtk.h>

#define MAX_RETRY 10

/**
 * @brief Assign the IR code to the Color
 * @param color color to be assigned
 * @param arduino the arduino controller
 * @return
 */
int match_color_with_IR(Color *color, ArduinoController *arduino);

/**
 * @brief Assign the IR code to the Function
 * @param funct Function to be assigned
 * @param arduino the arduino controller
 * @return
 */
int match_function_with_IR(Function *funct, ArduinoController *arduino);

//Manages the light wizard with GTK
void popup_light_wizard(GtkButton *button, gpointer user_data);




#endif //LIGHT_WIZARD_H
