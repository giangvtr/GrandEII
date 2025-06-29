/**
* @file popup_controller.c
 * @brief Contrôleur pour les popups du wizard et de la captation infrarouge/lumière.
 * @author malo
 * @date 2025-05-22
 */

#include "popup_controller.h"

#include "live_controller.h"
#include "../arduino/communication.h"
#include "../view/view_popup.h"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////  POPUP WIZARD  ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///Glabal value useful for the wiazrd
int group_selected =0;
const char* light_name;
Light* light_born;


//////////////////////////////////////////////////////////////////////////
//////////////////////////// PAGE 1 //////////////////////////////////////


/**
 * @brief Goes from the introduction page to page 1 of the wizard.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */

void on_btn_next_intro_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack || !context->builder) {
        g_warning("Contexte invalide pour btn_next_intro.");
        return;
    }

    gtk_stack_set_visible_child_name(context->stack, "page1");
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////// PAGE 2 //////////////////////////////////////

/**
 * @brief Proceeds from the name entry page to the group selection page.
 *        Also retrieves the name entered by the user.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_next_name_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack) {
        g_warning("Contexte invalide pour btn_next_name.");
        return;
    }

    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(context->builder, "saisie_name"));
    if (!entry) {
        g_warning("GtkEntry 'entry_light_name' introuvable.");
        return;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //Mettre ici la fonction qui définit le nom
    light_name = gtk_entry_get_text(entry);

    //g_print("Nom saisi : %s\n", light_name);
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    if (strlen(light_name) > 0 && strlen(light_name) < MAX_NAME_LENGTH){
        gtk_stack_set_visible_child_name(context->stack, "page2");
    }
}



/**
 * @brief Goes back to the introduction page from the name entry page.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_previous_name_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack) {
        g_warning("Contexte invalide pour btn_previous_name.");
        return;
    }

    gtk_stack_set_visible_child_name(context->stack, "page0");
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////// PAGE 3 //////////////////////////////////////


/**
 * @brief Proceeds to the first IR signal detection page.
 *        Also retrieves the selected group and initializes the light.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_next_group_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack || !context->builder) {
        g_warning("Contexte invalide pour btn_next_group.");
        return;
    }

    GtkSpinButton *spin = GTK_SPIN_BUTTON(gtk_builder_get_object(context->builder, "group_selector"));
    if (!spin) {
        g_warning("GtkSpinButton 'group_selector' introuvable.");
        return;
    }

    group_selected = gtk_spin_button_get_value_as_int(spin);
    g_print("Groupe sélectionné : %d\n", group_selected);

    //Create the new light
    light_born = alloc_light();
    initNewLight(light_born, light_name, INFRAROUGE);

    gtk_stack_set_visible_child_name(context->stack, "page3");
}



/**
 * @brief Goes back to the name selection page from the group selection page.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_previous_group_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack || !context->builder) {
        g_warning("Contexte invalide pour previous_duplicate1.");
        return;
    }

    gtk_stack_set_visible_child_name(context->stack, "page1");
}





//////////////////////////////////////////////////////////////////////////
//////////////////////////// PAGE 4 //////////////////////////////////////

/**
 * @brief Goes from the first IR signal capture page to the next page.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_next_duplicate1_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack || !context->builder) {
        g_warning("Contexte invalide pour btn_next_duplicate1.");
        return;
    }

    gtk_stack_set_visible_child_name(context->stack, "page4");
}

/**
 * @brief Goes back to the group selection page and frees the allocated light.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_previous_duplicate1_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack || !context->builder) {
        g_warning("Contexte invalide pour previous_duplicate1.");
        return;
    }
    //Before going to the previous page, we need to deallocate the light
    free(light_born);
    gtk_stack_set_visible_child_name(context->stack, "page2");
}


/**
 * @brief Captures the IR signal for a button.
 * @param button The clicked button.
 * @param user_data Unused.
 */
static void captation_funct(int functionName){
    //Check first if we have a live controller
    if (!isValidLiveController(global_live_controller)){
        fprintf(stderr, "Arduino is not linked or live controller is not initialized\n");
        return;
    }
    GtkBuilder *popup_builder = gtk_builder_new_from_file(UI_PATH);
    if (!popup_builder) {
        g_warning("Erreur de chargement du builder UI.");
        return;
    }

    GtkWidget *popup_capation = GTK_WIDGET(gtk_builder_get_object(popup_builder, "popup_captation"));
    if (!popup_capation || !GTK_IS_WINDOW(popup_capation)) {
        g_warning("popup_captation introuvable ou invalide.");
        g_object_unref(popup_builder);
        return;
    }

    gtk_widget_show_all(popup_capation);

    //Read IR CODE
    int ret_arduino = read_arduino(global_live_controller->arduino_controller);
    if (ret_arduino!=0){
        fprintf(stderr, "Failed to get the IR code\n");
        return;
    }
    Function* temp_funct = alloc_function();
    set_funct(temp_funct, functionName);
    memcpy(temp_funct->ir_code, global_live_controller->arduino_controller->ir_last_code, IR_CODE_LENGTH);
    int ret_insert = insertFirst(light_born->function_list, temp_funct, LIST_TYPE_FUNCTION, compareFunction);
    if (ret_insert==0){
        fprintf(stderr, "Failed to insert the new function!\n");
        return;
    }
}

/**
 * @brief Call to the function captation_funct with parameter ON
 * @param button The clicked button.
 * @param user_data Unused.
 */
void popup_captation_on(GtkButton *button, gpointer user_data) {
    captation_funct(ON);
}



/**
 * @brief Call to the function captation_funct with parameter OFF
 * @param button The clicked button.
 * @param user_data Unused.
 */
void popup_captation_off(GtkButton *button, gpointer user_data) {
    captation_funct(OFF);
}


/**
 * @brief Call to the function captation_funct with parameter BRIGHTNESS_MORE
 * @param button The clicked button.
 * @param user_data Unused.
 */
void popup_captation_intensityplus(GtkButton *button, gpointer user_data) {
    captation_funct(BRIGHTNESS_MORE);
}



/**
 * @brief Call to the function captation_funct with parameter BRIGHTNESS_LESS
 * @param button The clicked button.
 * @param user_data Unused.
 */
void popup_captation_intensitymoins(GtkButton *button, gpointer user_data) {
    captation_funct(BRIGHTNESS_LESS);
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////// PAGE 5 //////////////////////////////////////


/**
 * @brief Goes from the second IR signal capture page to the next page.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_next_duplicate2_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack || !context->builder) {
        g_warning("Contexte invalide pour btn_next_duplicate2.");
        return;
    }

    gtk_stack_set_visible_child_name(context->stack, "page5");
}



/**
 * @brief Goes back to the first IR detection page.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_previous_duplicate2_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack || !context->builder) {
        g_warning("Contexte invalide pour previous_duplicate2.");
        return;
    }

    gtk_stack_set_visible_child_name(context->stack, "page3");
}



void popup_captation_light(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    GtkBuilder *builder = context->builder;

    // Récupère le GtkColorButton
    GtkWidget *btn_color_chooser = GTK_WIDGET(gtk_builder_get_object(builder, "btn_color_chooser"));
    if (!btn_color_chooser || !GTK_IS_COLOR_BUTTON(btn_color_chooser)) {
        g_warning("GtkColorButton 'color_chooser_button' introuvable ou invalide.");
        return;
    }

    // Récupère la couleur sélectionnée
    GdkRGBA color;
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(btn_color_chooser), &color);

    g_print("Couleur sélectionnée : R=%.2f G=%.2f B=%.2f\n", color.red, color.green, color.blue); //BOOOORISSSS


    //Check first if we have a live controller
    if (!isValidLiveController(global_live_controller)){
        fprintf(stderr, "Arduino is not linked or live controller is not initialized\n");
        return;
    }

    GtkBuilder *popup_builder = gtk_builder_new_from_file(UI_PATH);
    if (!popup_builder) {
        g_warning("Erreur de chargement du builder UI.");
        return;
    }

    GtkWidget *popup_capation = GTK_WIDGET(gtk_builder_get_object(popup_builder, "popup_captation"));
    if (!popup_capation || !GTK_IS_WINDOW(popup_capation)) {
        g_warning("popup_captation introuvable ou invalide.");
        g_object_unref(popup_builder);
        return;
    }

    gtk_widget_show_all(popup_capation);

    //Read IR CODE
    int ret_arduino = read_arduino(global_live_controller->arduino_controller);
    if (ret_arduino!=0){
        fprintf(stderr, "Failed to get the IR code\n");
        return;
    }

    Color* temp_color = alloc_color();
    set_color(temp_color, color.red*255, color.green*255, color.blue*255);

    memcpy(temp_color->ir_code, global_live_controller->arduino_controller->ir_last_code, IR_CODE_LENGTH);
    int ret_insert = insertFirst(light_born->color_list, temp_color, LIST_TYPE_COLOR, compareFunction);
    if (ret_insert==0){
        fprintf(stderr, "Failed to insert the new function!\n");
        return;
    }
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////// PAGE 6 //////////////////////////////////////


/**
 * @brief Goes from the third IR signal capture page to the final page.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_next_duplicate3_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack || !context->builder) {
        g_warning("Contexte invalide pour btn_next_duplicate2.");
        return;
    }

    gtk_stack_set_visible_child_name(context->stack, "page6");
}



/**
 * @brief Goes back to the second IR detection page.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_previous_duplicate3_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack || !context->builder) {
        g_warning("Contexte invalide pour previous_duplicate2.");
        return;
    }

    gtk_stack_set_visible_child_name(context->stack, "page4");
}




/**
 * @brief Call to the function captation_funct with parameter FADE1
 * @param button The clicked button.
 * @param user_data Unused.
 */
void btn_add_fade1_clicked(GtkButton *button, gpointer user_data) {
    captation_funct(FADE1);
}


/**
 * @brief Call to the function captation_funct with parameter FADE2
 * @param button The clicked button.
 * @param user_data Unused.
 */
void btn_add_fade2_clicked(GtkButton *button, gpointer user_data) {
    captation_funct(FADE2);
}



/**
 * @brief Call to the function captation_funct with parameter FADE3
 * @param button The clicked button.
 * @param user_data Unused.
 */
void btn_add_fade3_clicked(GtkButton *button, gpointer user_data) {
    captation_funct(FADE3);
}



/**
 * @brief Call to the function captation_funct with parameter AUTO1
 * @param button The clicked button.
 * @param user_data Unused.
 */
void btn_add_auto1_clicked(GtkButton *button, gpointer user_data) {
    captation_funct(AUTO1);
}


/**
 * @brief Call to the function captation_funct with parameter AUTO2
 * @param button The clicked button.
 * @param user_data Unused.
 */
void btn_add_auto2_clicked(GtkButton *button, gpointer user_data) {
    captation_funct(AUTO2);
}



/**
 * @brief Call to the function captation_funct with parameter AUTO3
 * @param button The clicked button.
 * @param user_data Unused.
 */
void btn_add_auto3_clicked(GtkButton *button, gpointer user_data) {
    captation_funct(AUTO3);
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////// PAGE 7 //////////////////////////////////////

/**
 * @brief Goes back to the previous page from the final wizard page.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */

void on_btn_previous_end_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->stack || !context->builder) {
        g_warning("Contexte invalide pour previous_duplicate2.");
        return;
    }

    gtk_stack_set_visible_child_name(context->stack, "page5");
}

/**
 * @brief Final "Done" button callback.
 *        Hides the wizard popup and adds the light to the selected group.
 * @param button The clicked button.
 * @param user_data The wizard context (WizardContext *).
 */
void on_btn_popup_end_clicked(GtkButton *button, gpointer user_data) {
    WizardContext *context = (WizardContext *)user_data;
    if (!context || !context->builder) {
        g_warning("Contexte invalide pour fermeture du popup wizard.");
        return;
    }

    GtkWidget *popup = GTK_WIDGET(gtk_builder_get_object(context->builder, "popup_wizard"));
    if (!popup || !GTK_IS_WINDOW(popup)) {
        g_warning("popup_wizard introuvable ou invalide.");
        return;
    }

    int ret_code = addLighttoGroup(global_live_controller->manager->groups[group_selected-1], light_born);
    if (ret_code==-1){
        fprintf(stderr, "Erreurr durant addLighttoGroup.\n");
    }
    printGroup(global_live_controller->manager->groups[group_selected-1]);
    gtk_widget_hide(GTK_WIDGET(popup)); //ou destoy à voir
}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////  POPUP CAPTATION  /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Opens the validation IR captation popup.
 * @param button The clicked button.
 * @param user_data Unused.
 */
void popup_captation(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new_from_file(UI_PATH);
    if (!builder) {
        g_warning("Erreur lors du rechargement de l'UI");
        return;
    }

    GtkButton *btn_fermer_captation = GTK_BUTTON(gtk_builder_get_object(builder, "btn_fermer_parameter"));
    g_signal_connect(btn_fermer_captation, "clicked", G_CALLBACK(btn_fermer_captation_clicked), builder);


    GtkWidget *popup_captation = GTK_WIDGET(gtk_builder_get_object(builder, "popup_captation"));
    gtk_widget_show_all(popup_captation);
}




/**
 * @brief Closes the IR validation of captation popup.
 * @param button The clicked button.
 * @param user_data The GtkBuilder * used to access the popup widget.
 */
void btn_fermer_captation_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);
    if (!builder) {
        g_warning("Builder invalide (NULL) dans btn_fermer_captation_clicked.");
        return;
    }

    GtkWidget *popup_captation = GTK_WIDGET(gtk_builder_get_object(builder, "popup_captation"));
    if (!popup_captation) {
        g_warning("popup_captation introuvable dans le fichier .ui.");
        return;
    }

    gtk_widget_hide(popup_captation);
}


