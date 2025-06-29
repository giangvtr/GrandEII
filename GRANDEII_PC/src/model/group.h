//
// Created by giangvu on 3/13/25.
//

/**
 * @file group.h
 * @brief Gestion des groupes de lumières (brightness, couleur, contenu).
 *
 * Ce module fournit une abstraction pour gérer des groupes de lumières.
 * Chaque groupe a un nom, un identifiant unique, un niveau de luminosité,
 * une couleur partagée, et une liste de lumières.
 *
 * Les fonctions proposées permettent d’allouer, initialiser, modifier,
 * comparer, afficher et libérer des groupes.
**/

#ifndef GROUP_H
#define GROUP_H

#include "light.h"
#include "color.h"
#include "list_common.h"

#define MAX_LIGHTS 10 /**< Nombre maximal de lumières par groupe */

/**
 * @struct Group
 * @brief Représente un groupe de lumières avec une couleur et une luminosité communes.
 *
 * Cette structure permet de manipuler des groupes homogènes de lumières,
 * utiles dans le contrôle d’éclairage scénique ou domotique.
 */
typedef struct {
    char *name;                  /**< Nom du groupe */
    List *lights;               /**< Liste des lumières appartenant au groupe */
    int nbLights;               /**< Nombre de lumières actuellement dans le groupe */
    int group_brightness;       /**< Luminosité globale du groupe */
    Color *group_color;         /**< Couleur assignée au groupe */
    uint8_t id;                 /**< Identifiant unique du groupe */
} Group;

/**
 * @brief Alloue dynamiquement un groupe vide.
 *
 * @return Un pointeur vers une structure Group allouée, ou NULL si échec.
 */
Group* allocGroup();

/**
 * @brief Initialise un groupe avec un nom, une luminosité et une couleur.
 *
 * Cette fonction suppose que le groupe a été alloué avant.
 *
 * @param group Pointeur vers le groupe à initialiser.
 * @param name Nom à assigner au groupe.
 * @param group_brightness Luminosité initiale du groupe.
 * @param group_color Couleur initiale du groupe.
 * @return 0 si succès, -1 si erreur (allocation ou pointeurs NULL).
 */
int initGroup(Group *group, const char *name, int group_brightness, Color *group_color);

/**
 * @brief Ajoute une lumière au groupe.
 *
 * Vérifie si le nombre maximum est atteint avant insertion.
 *
 * @param group Pointeur vers le groupe.
 * @param light Pointeur vers la lumière à ajouter.
 * @return 0 si succès, -1 si erreur (NULL, dépassement, insertion échouée).
 */
int addLighttoGroup(Group *group, Light *light);

/**
 * @brief Vérifie si le groupe est vide (aucune lumière).
 *
 * @param group Pointeur constant vers le groupe.
 * @return 1 si vide, 0 si non vide, -1 si erreur.
 */
int isEmptyGroup(const Group *group);

/**
 * @brief Modifie la luminosité du groupe.
 *
 * @param group Pointeur vers le groupe.
 * @param brightness Nouvelle valeur de luminosité à affecter.
 */
void setGroupBrightness(Group *group, int brightness);

/**
 * @brief Modifie la couleur du groupe.
 *
 * @param group Pointeur vers le groupe.
 * @param color Nouvelle couleur à affecter.
 */
void setGroupColor(Group *group, Color *color);

/**
 * @brief Récupère le nom du groupe.
 *
 * @param group Pointeur constant vers le groupe.
 * @return Chaîne de caractères du nom, ou NULL si group est NULL.
 */
const char *getGroupName(const Group *group);

/**
 * @brief Récupère l'identifiant unique du groupe.
 *
 * @param group Pointeur constant vers le groupe.
 * @return ID du groupe, ou 0 si le pointeur est NULL.
 */
uint8_t getGroupID(const Group *group);

/**
 * @brief Récupère la couleur du groupe.
 *
 * @param group Pointeur constant vers le groupe.
 * @return Pointeur vers la structure Color, ou NULL si group est NULL.
 */
Color *getGroupColor(const Group *group);

/**
 * @brief Compare deux groupes.
 *
 * D'abord par ID (plus rapide et unique), puis par nom si nécessaire.
 *
 * @param groupA Premier groupe.
 * @param groupB Deuxième groupe.
 * @return 1 s’ils sont équivalents, 0 sinon.
 */
int compareGroup(const Group *groupA, const Group *groupB);

/**
 * @brief Affiche les informations du groupe dans la console.
 *
 * Nom, ID, luminosité, couleur, nombre de lumières, et détails des lumières.
 *
 * @param group Pointeur vers le groupe à afficher.
 * @return 0 si succès, -1 si erreur.
 */
int printGroup(Group *group);

/**
 * @brief Libère la mémoire allouée au groupe et à ses composants.
 *
 * Libère le nom, la liste de lumières, puis la structure du groupe elle-même.
 *
 * @param group Pointeur vers le groupe à libérer.
 */
void deleteGroup(Group *group);

#endif // GROUP_H
