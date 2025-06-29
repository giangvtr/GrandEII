/**
*@file controller.c
*@author Malo
*@brief Dans ce fichier on récupère les signaux émis par la view et on déclenche les fonctions apropriées
*/


#include "controller.h"

#include "live_controller.h"
#include "../view/view.h"
#include "../view/effect.h"


/**
 * @brief Handles the "Add" button click, switches to page0 and notifies the toolbar.
 * @param button The GTK button clicked.
 * @param user_data A pointer to the GtkStack used for navigation.
 */
void btn_add_clicked(GtkButton *button, gpointer user_data) {
    GtkStack *stack = GTK_STACK(user_data);
    if (!stack) {
        g_warning("GtkStack non défini.");
        return;
    }
    gtk_stack_set_visible_child_name(stack, "page0");
    on_toolbar_button_clicked(1);
}




/**
 * @brief Handles the "Patch" button click, switches to page2 and notifies the toolbar.
 * @param button The GTK button clicked.
 * @param user_data A pointer to the GtkStack used for navigation.
 */
void btn_patch_clicked(GtkButton *button, gpointer user_data) {
    GtkStack *stack = GTK_STACK(user_data);
    if (!stack) {
        g_warning("GtkStack non défini.");
        return;
    }
    gtk_stack_set_visible_child_name(stack, "page2");
    on_toolbar_button_clicked(3);
}




/**
 * @brief Handles the "Live" button click, switches to page1 and notifies the toolbar.
 * @param button The GTK button clicked.
 * @param user_data A pointer to the GtkStack used for navigation.
 */
void btn_live_clicked(GtkButton *button, gpointer user_data) {
    GtkStack *stack = GTK_STACK(user_data);
    if (!stack) {
        g_warning("GtkStack non défini.");
        return;
    }
    gtk_stack_set_visible_child_name(stack, "page1");
    on_toolbar_button_clicked(2);
}



/**
 * @brief Activates group 1 in live mode and sets current group index.
 */
void btn_group1_clicked(GtkButton *button, gpointer user_data) {
     g_print("Groupe 1\n");
    global_live_controller->current_group_index = 0;
    on_group_button_clicked(1);
}




/**
 * @brief Activates group 2 in live mode and sets current group index.
 */
void btn_group2_clicked(GtkButton *button, gpointer user_data) {
    g_print("Groupe 2\n");
    global_live_controller->current_group_index = 1;
    on_group_button_clicked(2);
}




/**
 * @brief Activates group 3 in live mode and sets current group index.
 */
void btn_group3_clicked(GtkButton *button, gpointer user_data) {
    g_print("Groupe 3\n");
    global_live_controller->current_group_index = 2;
    on_group_button_clicked(3);
}
guint border_timeout_id = 0;




/**
 * @brief Stops any active timers such as RGB tuning animation.
 */
void stop_all_timers() {
    if (border_timeout_id != 0) {
        g_source_remove(border_timeout_id);
        border_timeout_id = 0;
    }
}




/**
 * @brief Resets GTK button styles to default appearance using CSS.
 */
void reset_button_styles_to_default() {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "button {\n"
        "  border: 0.8px solid white;\n"
        "  border-radius: 6px;\n"
        "  padding: 6px 12px;\n"
        "  background: #050505;\n"
        "}\n"
        "label {\n"
        "  color: white;\n"
        "}\n"
        "window {\n"
        "  background-color: #101010;\n"
        "}\n",
        -1, NULL);

    GtkStyleContext *context = gtk_style_context_new();
    GtkWidgetPath *path = gtk_widget_path_new();
    gtk_widget_path_append_type(path, GTK_TYPE_WINDOW);
    gtk_style_context_set_path(context, path);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER
    );

    g_object_unref(provider);
    gtk_widget_path_free(path);
    g_object_unref(context);
}
int tunning = 0; //0 si pas actuellemnt pas de RGB 1 autrement





/**
 * @brief Toggles RGB tuning mode on/off when the tuning button is clicked.
 */
void btn_tunning_clicked(GtkButton *button, gpointer user_data) {
    g_print("Bouton TUNNING cliqué\n");
    if (tunning==0) {
        on_tunning(button, user_data);
        tunning = 1;
    } else {
        off_tunning(button, user_data);
        tunning = 0;
    }
}




/**
 * @brief Starts the RGB tuning animation loop.
 */
void on_tunning(GtkButton *button, gpointer user_data) {
    g_print("Slide Mode Tunning: ON\n");

    if (border_timeout_id == 0) {
        border_timeout_id = g_timeout_add(400, update_border_color, NULL);
    }
}




/**
 * @brief Stops the RGB tuning animation loop and resets button styles.
 */
void off_tunning(GtkButton *button, gpointer user_data) {
    g_print("Slide Mode Tunning: OFF\n");

    if (border_timeout_id != 0) {
        g_source_remove(border_timeout_id);
        border_timeout_id = 0;
    }

    // Réinitialiser le style par défaut
    reset_button_styles_to_default();
}




/**
 * @brief Displays an error dialog with the provided message.
 * @param message The message to display in the dialog.
 */
void show_error_dialog(const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}




/**
 * @brief Opens a file chooser dialog for selecting existing .deiivice files.
 * @return The selected filename (must be freed by the caller), or NULL if cancelled or invalid.
 */
char* btn_add_existing_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    char *filename = NULL;

    dialog = gtk_file_chooser_dialog_new("Select a file",
                                         NULL,
                                         action,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Open", GTK_RESPONSE_ACCEPT,
                                         NULL);

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "GrandEII files");
    gtk_file_filter_add_pattern(filter, "*.deiivice");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        if (g_str_has_suffix(filename, ".deiivice")) {
            gtk_widget_destroy(dialog);
            return filename;
        } else {
            show_error_dialog("File not compatible, please select a correct .deevice file.");
            g_free(filename);
            filename = NULL;
        }
    }

    gtk_widget_destroy(dialog);
    return NULL;
}



/**
 * @brief Displays the parameters popup window and connects its buttons.
 */
void btn_param_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new_from_file(UI_PATH);
    if (!builder) {
        g_warning("Erreur lors du rechargement de l'UI");
        return;
    }
    GtkButton *btn_tunning= GTK_BUTTON(gtk_builder_get_object(builder, "btn_tunning"));
    g_signal_connect(btn_tunning, "clicked", G_CALLBACK(btn_tunning_clicked), builder);

    GtkButton *btn_fermer = GTK_BUTTON(gtk_builder_get_object(builder, "btn_fermer_parameter"));
    g_signal_connect(btn_fermer, "clicked", G_CALLBACK(btn_fermer_parameter_clicked), builder);


    GtkWidget *param_window = GTK_WIDGET(gtk_builder_get_object(builder, "popup_param"));
    gtk_widget_show_all(param_window);
}


/**
 * @brief Closes (hides) the parameter popup window.
 */
void btn_fermer_parameter_clicked(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);
    if (!builder) {
        g_warning("Builder invalide (NULL) dans btn_fermer_parameter_clicked.");
        return;
    }

    GtkWidget *popup = GTK_WIDGET(gtk_builder_get_object(builder, "popup_param"));
    if (!popup) {
        g_warning("popup_param introuvable dans le fichier .ui.");
        return;
    }

    gtk_widget_hide(popup); // utilise hide pour juste le masquer
    // gtk_widget_destroy(popup);
}







