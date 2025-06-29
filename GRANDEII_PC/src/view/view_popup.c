/**
* @file view_popup.c
 * @brief Implementation of the Light Wizard popup view and its signal bindings.
 *
 * This file defines the `popup_light_wizard` function, which loads the popup UI,
 * initializes buttons and their callbacks, and sets up the wizard context for step-by-step
 * configuration of a light fixture.
 *
 * @author malo
 * @date 25/04/25
 */

#include <gtk/gtk.h>
#include "../view/view_popup.h"
#include "../controller/controller.h"
#include "../controller/popup_controller.h"



/**
 * @brief Initializes and displays the Light Wizard popup.
 *
 * This function loads the GtkBuilder UI from the .ui file and sets up all widgets,
 * buttons, and signal connections required for navigating through the wizard steps.
 * It also initializes a WizardContext structure which is passed to all the
 * callbacks for contextual access to widgets and data.
 *
 * @param button The GtkButton that triggered this popup (can be NULL).
 * @param user_data Optional user data passed to the callback (can be NULL).
 */
void popup_light_wizard(GtkButton *button, gpointer user_data) {
    GtkBuilder *popup_builder = gtk_builder_new_from_file(UI_PATH);
    if (!popup_builder) {
        g_warning("Erreur de chargement du builder UI.");
        return;
    }

    GtkWidget *popup = GTK_WIDGET(gtk_builder_get_object(popup_builder, "popup_wizard"));
    if (!popup || !GTK_IS_WINDOW(popup)) {
        g_warning("popup_wizard introuvable ou invalide.");
        return;
    }

    GtkStack *wizard_stack = GTK_STACK(gtk_builder_get_object(popup_builder, "popup_stack"));
    if (!wizard_stack || !GTK_IS_STACK(wizard_stack)) {
        g_warning("GtkStack popup_stack introuvable.");
        return;
    }

    // Création du contexte à transmettre aux callbacks
    WizardContext *context = g_new0(WizardContext, 1);
    context->stack = wizard_stack;
    context->builder = popup_builder;

    // Récupération des boutons
    GtkButton *btn_next_intro = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_next_intro"));

    GtkButton *btn_next_name = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_next_name"));
    GtkButton *btn_previous_name = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_previous_name"));

    GtkButton *btn_next_group = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_next_group"));
    GtkButton *btn_previous_group = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_previous_group"));
    GtkSpinButton *spin_selector = GTK_SPIN_BUTTON(gtk_builder_get_object(context->builder, "group_selector"));


    GtkButton *btn_next_duplicate1 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_next_duplicate1"));
    GtkButton *btn_previous_duplicate1 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_previous_duplicate1"));
    GtkButton *btn_configure_on = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_configure_on"));
    GtkButton *btn_configure_off = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_configure_off"));
    GtkButton *btn_configure_intensityplus = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_configure_intensityplus"));
    GtkButton *btn_configure_intensitymoins = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_configure_intensitymoins"));

    GtkButton *btn_next_duplicate2 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_next_duplicate2"));
    GtkButton *btn_previous_duplicate2 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_previous_duplicate2"));
    GtkButton *btn_addcolor = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_addcolor"));
    GtkWidget *btn_color_chooser = GTK_WIDGET(gtk_builder_get_object(popup_builder, "btn_color_chooser"));

    GtkButton *btn_next_duplicate3 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_next_duplicate3"));
    GtkButton *btn_previous_duplicate3 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_previous_duplicate3"));
    GtkButton *btn_add_fade1 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_add_fade1"));
    GtkButton *btn_add_fade2 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_add_fade2"));
    GtkButton *btn_add_fade3 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_add_fade3"));
    GtkButton *btn_add_auto1 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_add_auto1"));
    GtkButton *btn_add_auto2 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_add_auto2"));
    GtkButton *btn_add_auto3 = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_add_auto3"));


    GtkButton *btn_popup_end = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_popup_end"));
    GtkButton *btn_previous_end = GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_previous_end"));

    GtkButton *btn_fermer_captation= GTK_BUTTON(gtk_builder_get_object(popup_builder, "btn_fermer_captation"));


    // Connexion des signaux
    g_signal_connect(btn_next_intro, "clicked", G_CALLBACK(on_btn_next_intro_clicked), context);

    g_signal_connect(btn_next_name, "clicked", G_CALLBACK(on_btn_next_name_clicked), context);
    g_signal_connect(btn_previous_name, "clicked", G_CALLBACK(on_btn_previous_name_clicked), context);

    g_signal_connect(btn_next_group, "clicked", G_CALLBACK(on_btn_next_group_clicked), context);
    g_signal_connect(btn_previous_group, "clicked", G_CALLBACK(on_btn_previous_group_clicked), context);


    g_signal_connect(btn_next_duplicate1, "clicked", G_CALLBACK(on_btn_next_duplicate1_clicked), context);
    g_signal_connect(btn_previous_duplicate1, "clicked", G_CALLBACK(on_btn_previous_duplicate1_clicked), context);
    g_signal_connect(btn_configure_on, "clicked", G_CALLBACK(popup_captation_on), context);
    g_signal_connect(btn_configure_off, "clicked", G_CALLBACK(popup_captation_off), context);

    g_signal_connect(btn_next_duplicate2, "clicked", G_CALLBACK(on_btn_next_duplicate2_clicked), context);
    g_signal_connect(btn_previous_duplicate2, "clicked", G_CALLBACK(on_btn_previous_duplicate2_clicked), context);
    g_signal_connect(btn_addcolor, "clicked", G_CALLBACK(popup_captation_light), context);
    g_signal_connect(btn_configure_intensityplus, "clicked", G_CALLBACK(popup_captation_intensityplus), context);
    g_signal_connect(btn_configure_intensitymoins, "clicked", G_CALLBACK(popup_captation_intensitymoins), context);

    g_signal_connect(btn_next_duplicate3, "clicked", G_CALLBACK(on_btn_next_duplicate3_clicked), context);
    g_signal_connect(btn_previous_duplicate3, "clicked", G_CALLBACK(on_btn_previous_duplicate3_clicked), context);
    g_signal_connect(btn_add_fade1, "clicked", G_CALLBACK(btn_add_fade1_clicked), context);
    g_signal_connect(btn_add_fade2, "clicked", G_CALLBACK(btn_add_fade2_clicked), context);
    g_signal_connect(btn_add_fade3, "clicked", G_CALLBACK(btn_add_fade3_clicked), context);
    g_signal_connect(btn_add_auto1, "clicked", G_CALLBACK(btn_add_auto1_clicked), context);
    g_signal_connect(btn_add_auto2, "clicked", G_CALLBACK(btn_add_auto2_clicked), context);
    g_signal_connect(btn_add_auto3, "clicked", G_CALLBACK(btn_add_auto3_clicked), context);


    g_signal_connect(btn_popup_end, "clicked", G_CALLBACK(on_btn_popup_end_clicked), context);
    g_signal_connect(btn_previous_end, "clicked", G_CALLBACK(on_btn_previous_end_clicked), context);

    g_signal_connect(btn_fermer_captation, "clicked", G_CALLBACK(btn_fermer_captation_clicked), context);


    gtk_widget_show_all(popup);
}
