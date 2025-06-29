//
// Created by boris on 5/18/25.
//

#ifndef LIVE_CONTROLLER_H
#define LIVE_CONTROLLER_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../model/group_manager.h"
#include "../model/light.h"
#include "../model/list_common.h"
#include "../model/light.h"
#include <gtk/gtk.h>

#define INTERVAL_UPDATE 0.1

typedef struct{
    GroupManager *manager;
    ArduinoController *arduino_controller;
    int current_group_index;
} LiveController;

extern LiveController *global_live_controller;

/**
 * @brief Allocates and create the live controller
 * @return A pointer to the newly live controller
 */
LiveController *createLiveController();

/**
 * @brief Free the live controller
 * @param controller the live controller to be freed
 */
void deleteLiveController(LiveController *controller);

/**
 * @brief Send to all the light of the group of the current livecontroller group the IR code of the color passed in params
 * @param controller The live controller
 * @param color The color to be send
 * @return 1 if successful 0 else
 */
int updateColor(LiveController *controller, Color *color);

/**
 * @brief Send to all the light of the group of the current livecontroller group the IR ocde of the function passed in params
 * @param controller The live controller
 * @param funct_name The function to be send
 * @return 1 if successful 0 else
 */
int updateFunction(LiveController *controller, FunctionName funct_name);

/**
 * @brief Checks if the liveController is a valid one
 * @param controller the live controller
 * @return 0b00 if nothing is valid, 1 if the arduino is opened
 */
int isValidLiveController(LiveController *controller);

/**
 * @brief Function is called when any button is clicked in the live page
 * @param button The referance on the button
 * @param user_data some data that GTK is using
 */
void on_effect_button_clicked(GtkButton *button, gpointer user_data);
void on_switch_toggled(GObject *object, GParamSpec *pspec, gpointer user_data);
void on_scale_value_changed(GtkRange *range, gpointer user_data);


#endif //LIVE_CONTROLLER_H
