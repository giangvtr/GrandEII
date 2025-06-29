//
// Created by malo on 22/05/25.
//

#ifndef POPUP_CONTROLLER_H
#define POPUP_CONTROLLER_H

#include "../arduino/communication.h"
#include "../model/color.h"
#include "../view/view.h"
#include <gtk/gtk.h>

//Fonctions pour se balader entre les pages :
void on_btn_next_intro_clicked(GtkButton *button, gpointer user_data) ;

void on_btn_next_name_clicked(GtkButton *button, gpointer user_data);
void on_btn_previous_name_clicked(GtkButton *button, gpointer user_data);

void on_btn_next_group_clicked(GtkButton *button, gpointer user_data);
void on_btn_previous_group_clicked(GtkButton *button, gpointer user_data);

void on_btn_next_duplicate1_clicked(GtkButton *button, gpointer user_data);
void on_btn_previous_duplicate1_clicked(GtkButton *button, gpointer user_data);
void popup_captation_on(GtkButton *button, gpointer user_data);
void popup_captation_off(GtkButton *button, gpointer user_data);

void on_btn_next_duplicate2_clicked(GtkButton *button, gpointer user_data);
void on_btn_previous_duplicate2_clicked(GtkButton *button, gpointer user_data);
void popup_captation_light(GtkButton *button, gpointer user_data);
void popup_captation_intensityplus(GtkButton *button, gpointer user_data);
void popup_captation_intensitymoins(GtkButton *button, gpointer user_data);

void on_btn_next_duplicate3_clicked(GtkButton *button, gpointer user_data);
void on_btn_previous_duplicate3_clicked(GtkButton *button, gpointer user_data);
void btn_add_fade1_clicked(GtkButton *button, gpointer user_data);
void btn_add_fade2_clicked(GtkButton *button, gpointer user_data);
void btn_add_fade3_clicked(GtkButton *button, gpointer user_data);
void btn_add_auto1_clicked(GtkButton *button, gpointer user_data);
void btn_add_auto2_clicked(GtkButton *button, gpointer user_data);
void btn_add_auto3_clicked(GtkButton *button, gpointer user_data);

void on_btn_popup_end_clicked(GtkButton *button, gpointer user_data);
void on_btn_previous_end_clicked(GtkButton *button, gpointer user_data);



void btn_fermer_captation_clicked(GtkButton *button, gpointer user_data);
void on_annuler_lcaptation_clicked(GtkButton *button, gpointer user_data);
void signalIR_color_recu(GtkButton *button, gpointer user_data);


#endif //POPUP_CONTROLLER_H
