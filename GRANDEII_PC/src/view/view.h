#include <gtk/gtk.h>
#include "../controller/light_wizard.h"

//Dans view.c
void apply_css(void);
void on_activate(GtkApplication *app, gpointer user_data);
int main_view(int argc, char **argv);
void set_gras_toolbar(GtkLabel *selected, GtkLabel *other1, GtkLabel *other2);
void set_gras_group(GtkLabel *selected, GtkLabel *other1, GtkLabel *other2);
void on_toolbar_button_clicked(int position);
void on_group_button_clicked(int position);