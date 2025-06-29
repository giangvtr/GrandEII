//
// Created by malo on 25/04/25.
//

#ifndef VIEW_POPUP_H
#define VIEW_POPUP_H


#include "../arduino/communication.h"
#include "../model/color.h"
#include <gtk/gtk.h>

typedef struct {
    GtkStack *stack;
    GtkBuilder *builder;
} WizardContext;

//Manages the light wizard with GTK
void popup_light_wizard(GtkButton *button, gpointer user_data);









#endif //VIEW_POPUP_H
