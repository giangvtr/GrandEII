/**
* @file view.c
 * @brief GTK-based user interface for light control application.
 *
 * This file handles the initialization of the main window, toolbar, navigation buttons,
 * group selection, and light effect controls. It manages the UI behavior and interactions
 * via GTK signals and styling.
 *
 * @author Malo
 * @date 2025-05-27
 */



#include <gtk/gtk.h>
#include "view.h"
#include "../controller/controller.h"
#include "../controller/light_wizard.h"
#include "../controller/live_controller.h"
#include "effect.h"



// Déclaration globale pour les boutons de navigation
GtkLabel *txt_add;
GtkLabel *txt_live;
GtkLabel *txt_patch;
GtkLabel *txt_group1;
GtkLabel *txt_group2;
GtkLabel *txt_group3;
int position;







/**
 * @brief Sets one label in bold for the toolbar and resets the others to normal.
 *
 * If the selected label is the "Live" label, it applies a rainbow effect to each letter.
 *
 * @param selected The label to emphasize (bold).
 * @param other1 The first label to reset to normal style.
 * @param other2 The second label to reset to normal style.
 */
void set_gras_toolbar(GtkLabel *selected, GtkLabel *other1, GtkLabel *other2) {
    if (GTK_IS_LABEL(selected)) {
        const gchar *text = gtk_label_get_text(selected);

        gchar *markup;
        if (selected == txt_live) {
            // Effet rainbow (LIVE = 4 lettres → 4 couleurs)
            markup = g_strdup_printf(
                "<span weight=\"bold\" size=\"30000\">"
                "<span foreground=\"#FF0000\">L</span>"
                "<span foreground=\"#FF7F00\">i</span>"
                "<span foreground=\"#00FF00\">v</span>"
                "<span foreground=\"#0000FF\">e</span>"
                "</span>"
            );
        } else {
            markup = g_markup_printf_escaped("<span weight=\"bold\" size=\"20000\">%s</span>", text);
        }

        gtk_label_set_markup(selected, markup);
        g_free(markup);
    }

    if (GTK_IS_LABEL(other1)) {
        const gchar *text = gtk_label_get_text(other1);
        gchar *markup = g_markup_printf_escaped("<span weight=\"normal\" size=\"13000\">%s</span>", text);
        gtk_label_set_markup(other1, markup);
        g_free(markup);
    }

    if (GTK_IS_LABEL(other2)) {
        const gchar *text = gtk_label_get_text(other2);
        gchar *markup = g_markup_printf_escaped("<span weight=\"normal\" size=\"13000\">%s</span>", text);
        gtk_label_set_markup(other2, markup);
        g_free(markup);
    }
}






/**
 * @brief Sets one label in bold for group selection and resets the others.
 *
 * @param selected The label to emphasize (bold).
 * @param other1 The first label to reset to normal style.
 * @param other2 The second label to reset to normal style.
 */
void set_gras_group(GtkLabel *selected, GtkLabel *other1, GtkLabel *other2) {
    if (GTK_IS_LABEL(selected)) {
        const gchar *text = gtk_label_get_text(selected);
        gchar *markup = g_markup_printf_escaped("<span weight=\"bold\" size=\"15000\">%s</span>", text);
        gtk_label_set_markup(selected, markup);
        g_free(markup);
    }

    if (GTK_IS_LABEL(other1)) {
        const gchar *text = gtk_label_get_text(other1);
        gchar *markup = g_markup_printf_escaped("<span weight=\"normal\" size=\"11000\">%s</span>", text);
        gtk_label_set_markup(other1, markup);
        g_free(markup);
    }

    if (GTK_IS_LABEL(other2)) {
        const gchar *text = gtk_label_get_text(other2);
        gchar *markup = g_markup_printf_escaped("<span weight=\"normal\" size=\"11000\">%s</span>", text);
        gtk_label_set_markup(other2, markup);
        g_free(markup);
    }
}



/**
 * @brief Handles toolbar button click logic and applies the corresponding style.
 *
 * Updates the bold styling based on the button selected.
 *
 * @param position Integer representing which toolbar button is selected:
 * 1 = Add, 2 = Live, 3 = Patch.
 */
void on_toolbar_button_clicked(int position) {
    if (position == 1)
        set_gras_toolbar(txt_add, txt_live, txt_patch);
    else if (position == 2)
        set_gras_toolbar(txt_live, txt_add, txt_patch);
    else if (position == 3)
        set_gras_toolbar(txt_patch, txt_add, txt_live);
}

/**
 * @brief Handles group button click logic and applies the corresponding style.
 *
 * Updates the bold styling for group buttons.
 *
 * @param position Integer representing which group button is selected:
 * 1 = Group1, 2 = Group2, 3 = Group3.
 */
void on_group_button_clicked(int position) {
    if (position == 1)
        set_gras_group(txt_group1, txt_group2, txt_group3);
    else if (position == 2)
        set_gras_group(txt_group2, txt_group1, txt_group3);
    else if (position == 3)
        set_gras_group(txt_group3, txt_group1, txt_group2);
}



/**
 * @brief Main GTK application activation function.
 *
 * Initializes the GTK UI from a builder file, sets up buttons, labels, and connects
 * all required signal callbacks for navigation, light controls, and parameter settings.
 *
 * @param app The GtkApplication instance.
 * @param user_data Pointer to user data (unused).
 */
void on_activate(GtkApplication *app, gpointer user_data) {
    apply_css();
    stop_all_timers();

    GtkBuilder *builder = gtk_builder_new_from_file(UI_PATH);
    if (!builder) {
        g_warning("Erreur: Impossible de charger le fichier .ui");
        return;
    }

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!window || !GTK_IS_WINDOW(window)) {
        g_warning("Erreur: 'main_window' introuvable ou invalide.");
        return;
    }
    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_builder_connect_signals(builder, NULL);

    GtkStack *stack = GTK_STACK(gtk_builder_get_object(builder, "stack"));
    if (!stack || !GTK_IS_STACK(stack)) {
        g_warning("Erreur: GtkStack introuvable.");
        return;
    }

    // Initialiser la barre de navigation
    GtkButton *btn_add = GTK_BUTTON(gtk_builder_get_object(builder, "btn_add"));
    g_signal_connect(btn_add, "clicked", G_CALLBACK(btn_add_clicked), stack);
    GtkButton *btn_live = GTK_BUTTON(gtk_builder_get_object(builder, "btn_live"));
    g_signal_connect(btn_live, "clicked", G_CALLBACK(btn_live_clicked), stack);
    GtkButton *btn_patch = GTK_BUTTON(gtk_builder_get_object(builder, "btn_patch"));
    g_signal_connect(btn_patch, "clicked", G_CALLBACK(btn_patch_clicked), stack);
    GtkButton *btn_param = GTK_BUTTON(gtk_builder_get_object(builder, "btn_param"));
    g_signal_connect(btn_param, "clicked", G_CALLBACK(btn_param_clicked), builder);

    txt_add = GTK_LABEL(gtk_builder_get_object(builder, "txt_add"));
    on_toolbar_button_clicked(1);
    txt_live = GTK_LABEL(gtk_builder_get_object(builder, "txt_live"));
    txt_patch = GTK_LABEL(gtk_builder_get_object(builder, "txt_patch"));

    // Initialiser la page "Add"
    GtkButton *btn_add_existing = GTK_BUTTON(gtk_builder_get_object(builder, "btn_add_existing"));
    g_signal_connect(btn_add_existing, "clicked", G_CALLBACK(btn_add_existing_clicked), NULL);
    GtkButton *btn_add_light_wizard = GTK_BUTTON(gtk_builder_get_object(builder, "btn_light_wizard"));
    g_signal_connect(btn_add_light_wizard, "clicked", G_CALLBACK(popup_light_wizard), NULL);

    // Initialiser la page "Live"
    txt_group1 = GTK_LABEL(gtk_builder_get_object(builder, "txt_group1"));
    on_group_button_clicked(1);
    txt_group2 = GTK_LABEL(gtk_builder_get_object(builder, "txt_group2"));
    txt_group3 = GTK_LABEL(gtk_builder_get_object(builder, "txt_group3"));

    GtkButton *btn_group1 = GTK_BUTTON(gtk_builder_get_object(builder, "btn_group1"));
    g_signal_connect(btn_group1, "clicked", G_CALLBACK(btn_group1_clicked), "btn_group1");
    GtkButton *btn_group2 = GTK_BUTTON(gtk_builder_get_object(builder, "btn_group2"));
    g_signal_connect(btn_group2, "clicked", G_CALLBACK(btn_group2_clicked), "btn_group2");
    GtkButton *btn_group3 = GTK_BUTTON(gtk_builder_get_object(builder, "btn_group3"));
    g_signal_connect(btn_group3, "clicked", G_CALLBACK(btn_group3_clicked), "btn_group3");

    GtkButton *btn_on_live = GTK_BUTTON(gtk_builder_get_object(builder, "btn_on_live"));
    g_signal_connect(btn_on_live, "clicked", G_CALLBACK(on_effect_button_clicked), "ON");
    GtkButton *btn_off_live = GTK_BUTTON(gtk_builder_get_object(builder, "btn_off_live"));
    g_signal_connect(btn_off_live, "clicked", G_CALLBACK(on_effect_button_clicked), "OFF");

    GtkButton *btn_plus_live = GTK_BUTTON(gtk_builder_get_object(builder, "btn_plus_live"));
    g_signal_connect(btn_plus_live, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_plus_live");
    GtkButton *btn_moins_live = GTK_BUTTON(gtk_builder_get_object(builder, "btn_moins_live"));
    g_signal_connect(btn_moins_live, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_moins_live");

    GtkButton *btn_red = GTK_BUTTON(gtk_builder_get_object(builder, "btn_red"));
    GtkButton *btn_yellow = GTK_BUTTON(gtk_builder_get_object(builder, "btn_yellow"));
    GtkButton *btn_green = GTK_BUTTON(gtk_builder_get_object(builder, "btn_green"));
    GtkButton *btn_blue = GTK_BUTTON(gtk_builder_get_object(builder, "btn_blue"));
    GtkButton *btn_white = GTK_BUTTON(gtk_builder_get_object(builder, "btn_white"));
    GtkButton *btn_purple = GTK_BUTTON(gtk_builder_get_object(builder, "btn_purple"));
    GtkButton *btn_orange = GTK_BUTTON(gtk_builder_get_object(builder, "btn_orange"));
    GtkButton *btn_brown = GTK_BUTTON(gtk_builder_get_object(builder, "btn_brown"));
    g_signal_connect(btn_red, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_red");
    g_signal_connect(btn_yellow, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_yellow");
    g_signal_connect(btn_green, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_green");
    g_signal_connect(btn_blue, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_blue");
    g_signal_connect(btn_white, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_white");
    g_signal_connect(btn_purple, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_purple");
    g_signal_connect(btn_orange, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_orange");
    g_signal_connect(btn_brown, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_brown");

    GtkButton *btn_auto1 = GTK_BUTTON(gtk_builder_get_object(builder, "btn_auto1"));
    g_signal_connect(btn_auto1, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_auto1");
    GtkButton *btn_auto2 = GTK_BUTTON(gtk_builder_get_object(builder, "btn_auto2"));
    g_signal_connect(btn_auto2, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_auto2");
    GtkButton *btn_auto3 = GTK_BUTTON(gtk_builder_get_object(builder, "btn_auto3"));
    g_signal_connect(btn_auto3, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_auto3");

    GtkButton *btn_fade1 = GTK_BUTTON(gtk_builder_get_object(builder, "btn_fade1"));
    g_signal_connect(btn_fade1, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_fade1");
    GtkButton *btn_fade2 = GTK_BUTTON(gtk_builder_get_object(builder, "btn_fade2"));
    g_signal_connect(btn_fade2, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_fade2");
    GtkButton *btn_fade3 = GTK_BUTTON(gtk_builder_get_object(builder, "btn_fade3"));
    g_signal_connect(btn_fade3, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_fade3");

    GtkButton *btn_reset = GTK_BUTTON(gtk_builder_get_object(builder, "btn_reset"));
    g_signal_connect(btn_reset, "clicked", G_CALLBACK(on_effect_button_clicked), "btn_reset");

    //Initialiser popup "paramètres"
    GtkButton *btn_tunning= GTK_BUTTON(gtk_builder_get_object(builder, "btn_tunning"));
    g_signal_connect(btn_tunning, "clicked", G_CALLBACK(btn_tunning_clicked), builder);
    GtkButton *btn_fermer_parameter= GTK_BUTTON(gtk_builder_get_object(builder, "btn_fermer_parameter"));
    g_signal_connect(btn_fermer_parameter, "clicked", G_CALLBACK(btn_fermer_parameter_clicked), builder);



    gtk_widget_show_all(window);

}






/**
 * @brief Entry point for the GTK view module.
 *
 * Initializes and runs the GTK application loop.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Application exit status code.
 */
int main_view(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.eii.grandeii", 0);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
