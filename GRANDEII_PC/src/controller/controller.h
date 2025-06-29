//
// Created by boris on 4/24/25.
//
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <gtk/gtk.h>

void btn_add_clicked(GtkButton *button, gpointer user_data);
void btn_live_clicked(GtkButton *button, gpointer user_data);
void btn_patch_clicked(GtkButton *button, gpointer user_data);
void btn_param_clicked(GtkButton *button, gpointer user_data);
void btn_group1_clicked(GtkButton *button, gpointer user_data);
void btn_group2_clicked(GtkButton *button, gpointer user_data);
void btn_group3_clicked(GtkButton *button, gpointer user_data);


void btn_tunning_clicked(GtkButton *button, gpointer user_data);
void on_tunning(GtkButton *button, gpointer user_data);
void off_tunning(GtkButton *button, gpointer user_data);

void btn_fermer_parameter_clicked(GtkButton *button, gpointer user_data);

void stop_all_timers();

void show_error_dialog(const char *message);
char* btn_add_existing_clicked(GtkButton *button, gpointer user_data);

typedef struct {
    GtkStack *stack;
    GtkButton *btn_add;
    GtkButton *btn_live;
    GtkButton *btn_patch;
} NavigationContext;




#endif //CONTROLLER_H
