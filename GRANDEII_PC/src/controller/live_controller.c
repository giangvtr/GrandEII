//
// Created by boris on 5/18/25.
//

#include "live_controller.h"
#include "../model/function.h"


LiveController* global_live_controller;



/**
 * @brief Creates and initializes a new LiveController instance.
 *
 * Allocates memory for a LiveController, sets the default current group index to 0,
 * initializes a group manager with 3 groups, and creates an Arduino controller.
 *
 * @return A pointer to the initialized LiveController, or NULL if allocation fails.
 */
LiveController *createLiveController(){
    LiveController *controller = malloc(sizeof(LiveController));
    if (controller == NULL){
        fprintf(stderr,"Erreur d'allocation mémoire pour LiveCOntroller\n");
        return NULL;
    }

    controller->manager = createGroupManager(3);
    controller->current_group_index = 0;

    controller->arduino_controller = create_arduino_controller();

    return controller;
}




/**
 * @brief Checks whether a given LiveController is valid and ready to use.
 *
 * A LiveController is considered valid if it is non-NULL, its Arduino controller is non-NULL,
 * and the Arduino connection is open.
 *
 * @param controller The LiveController to validate.
 * @return 1 if valid, 0 otherwise.
 */
int isValidLiveController(LiveController *controller){
    if (controller == NULL) return 0;
    if (controller->arduino_controller==NULL) return 0;
    if (controller->arduino_controller->isOpened) return 1;
    return 0;
}





/**
 * @brief Updates all lights in the current group with the nearest color to the given one.
 *
 * Iterates through the lights in the currently active group, finds the nearest available
 * color based on the light's functions, and sends the corresponding IR code via Arduino.
 *
 * @param controller The LiveController managing the lights.
 * @param color The target color to be approximated and applied to the lights.
 * @return 1 if update was successful, 0 otherwise.
 *
 * @note Assumes controller and color are both non-NULL.
 */
int updateColor(LiveController *controller, Color *color){
    if (color == NULL) return 0;
    if (controller==NULL) return 0;

    Group* current_group = controller->manager->groups[controller->current_group_index];

    //Set the global color
    current_group->group_color = color;

    setOnFirst(current_group->lights);
    for (int index_light=0; index_light < current_group->nbLights; index_light++){
        //Cycle through every light
        Light* current_light = current_group->lights->current->data;

        //Get the color of the light
        Color* light_color = getNearestColor(current_light, color);
        if (light_color==NULL) continue;//No function
        if (isValidLiveController(controller)){
            write_arduino(controller->arduino_controller, light_color->ir_code);
        }


        setOnNext(current_group->lights);
    }
    return 1;
}





/**
 * @brief Applies a given function to all lights in the current group.
 *
 * For each light in the active group, retrieves the specified function and sends the IR code
 * to the Arduino if valid.
 *
 * @param controller The LiveController managing the lights.
 * @param funct_name The function to apply (e.g., ON, OFF, FADE1, etc.).
 * @return 1 if function was applied, 0 otherwise.
 */
int updateFunction(LiveController *controller, FunctionName funct_name){
    if (controller==NULL) return 0;

    Group* current_group = controller->manager->groups[controller->current_group_index];

    setOnFirst(current_group->lights);
    for (int index_light=0; index_light < current_group->nbLights; index_light++){
        //Cycle through every light
        Light* current_light = current_group->lights->current->data;

        //Get the color of the light
        Function* light_funct = getFunction(current_light, funct_name);
        if (light_funct==NULL) continue;//No function
        print_function_info(light_funct);

        //Send IR code
        if (isValidLiveController(controller)){
            write_arduino(controller->arduino_controller, light_funct->ir_code);
        }

        setOnNext(current_group->lights);
    }
    return 1;
}



/**
 * @brief Frees all memory associated with a LiveController instance.
 *
 * Deletes the group manager, frees the Arduino controller, and deallocates
 * the controller structure itself.
 *
 * @param controller The LiveController to delete.
 */
void deleteLiveController(LiveController *controller){
    if (controller == NULL){
        return;
    }else{
        deleteGroupManager(controller->manager);
        free_arduino_controller(controller->arduino_controller);
        free(controller);
    }
}



/**
 * @brief Handles clicks on effect buttons in the live control UI.
 *
 * This function responds to various effect buttons pressed in the live interface by
 * either sending predefined IR codes to the Arduino for specific functions (e.g., ON, OFF,
 * brightness adjustment) or by creating colors dynamically and applying them to the current
 * group of lights through the LiveController.
 *
 * @param button The GtkButton that was clicked.
 * @param user_data A string (const char *) identifying which button was clicked.
 *
 * The following string values are expected:
 * - "ON", "OFF"
 * - "btn_moins_live", "btn_plus_live", "btn_reset"
 * - "btn_red", "btn_blue", "btn_yellow", "btn_white", "btn_purple", "btn_orange", "btn_brown"
 * - "btn_fade1", "btn_fade2", "btn_fade3"
 * - "btn_auto1", "btn_auto2", "btn_auto3"
 *
 * @note This function uses the global variable `global_live_controller`.
 * It creates temporary Color objects for RGB changes and frees them after use.
 *
 * @see updateFunction, updateColor, set_color, alloc_color, free_color
 */
void on_effect_button_clicked(GtkButton *button, gpointer user_data){
    const char *effect = (const char *)user_data;
    g_print("Effet cliqué: %s\n", effect);

    /*
     WARNING: it gets ugly
    */
    if (strcmp(effect, "ON") == 0){
        updateFunction(global_live_controller, ON);
    }else if (strcmp(effect, "OFF") == 0){
        updateFunction(global_live_controller, OFF);
    }else if (strcmp(effect, "btn_moins_live") == 0){
        updateFunction(global_live_controller, BRIGHTNESS_LESS);
    }else if (strcmp(effect, "btn_plus_live") == 0){
        updateFunction(global_live_controller, BRIGHTNESS_MORE);
    }else if (strcmp(effect, "btn_reset") == 0){
        for (int i=0; i<30; i++) updateFunction(global_live_controller, BRIGHTNESS_LESS);
    }else if (strcmp(effect, "btn_red") == 0){
        Color* red=alloc_color();
        set_color(red, 255,0,0);
        updateColor(global_live_controller, red);
        free_color(red);
    }else if (strcmp(effect, "btn_blue") == 0) {
        Color* blue = alloc_color();
        set_color(blue, 0, 0, 255);  // RGB for Blue
        updateColor(global_live_controller, blue);
        free_color(blue);
    } else if (strcmp(effect, "btn_yellow") == 0) {
        Color* yellow = alloc_color();
        set_color(yellow, 255, 255, 0);  // RGB for Yellow
        updateColor(global_live_controller, yellow);
        free_color(yellow);
    } else if (strcmp(effect, "btn_white") == 0) {
        Color* white = alloc_color();
        set_color(white, 255, 255, 255);  // RGB for White
        updateColor(global_live_controller, white);
        free_color(white);
    } else if (strcmp(effect, "btn_purple") == 0) {
        Color* purple = alloc_color();
        set_color(purple, 160, 32, 240);  // RGB for Purple
        updateColor(global_live_controller, purple);
        free_color(purple);
    } else if (strcmp(effect, "btn_orange") == 0) {
        Color* orange = alloc_color();
        set_color(orange, 255, 165, 0);  // RGB for Orange
        updateColor(global_live_controller, orange);
        free_color(orange);
    } else if (strcmp(effect, "btn_brown") == 0) {
        Color* brown = alloc_color();
        set_color(brown, 165, 42, 42);  // RGB for Brown
        updateColor(global_live_controller, brown);
        free_color(brown);
    } else if (strcmp(effect, "btn_fade1") == 0) {
        updateFunction(global_live_controller, FADE1);
    } else if (strcmp(effect, "btn_fade2") == 0) {
        updateFunction(global_live_controller, FADE2);
    } else if (strcmp(effect, "btn_fade3") == 0) {
        updateFunction(global_live_controller, FADE3);
    } else if (strcmp(effect, "btn_auto1") == 0) {
        updateFunction(global_live_controller, AUTO1);
    } else if (strcmp(effect, "btn_auto2") == 0) {
        updateFunction(global_live_controller, AUTO2);
    } else if (strcmp(effect, "btn_auto3") == 0) {
        updateFunction(global_live_controller, AUTO3);
    }
}





/**
 * @brief Callback for when a value on a GTK scale widget is changed.
 *
 * Prints the current value of the scale to standard output.
 *
 * @param range The GtkRange (e.g., GtkScale) widget.
 * @param user_data User-defined data (unused).
 */
void on_scale_value_changed(GtkRange *range, gpointer user_data)
{
    gdouble value = gtk_range_get_value(range);
    g_print("Valeur du scale : %.2f\n", value);
}




/**
 * @brief Callback for when a GtkSwitch is toggled.
 *
 * Logs the state of the switch (ON or OFF) to the console.
 *
 * @param object The GObject (GtkSwitch) being toggled.
 * @param pspec The GParamSpec associated with the signal (unused).
 * @param user_data User-defined data (unused).
 */
void on_switch_toggled(GObject *object, GParamSpec *pspec, gpointer user_data) {
    GtkSwitch *widget = GTK_SWITCH(object);
    gboolean active = gtk_switch_get_active(widget);
    g_print("Switch toggled: %s\n", active ? "ON" : "OFF");
}

