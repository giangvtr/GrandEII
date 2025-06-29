//
// Created by giangvu on 4/6/25.
//

#include "UTest-list.h"


// -------- SETUP / TEARDOWN --------
int setup_list_light(void **state){
    List *l = allocList();
    initList(l,LIST_TYPE_LIGHT);
    *state = l;
    return 0;
}

int setup_list_light_2light(void **state) {
    List *l = allocList();
    initList(l, LIST_TYPE_LIGHT);

    // First Light
    Light *light1 = alloc_light();
    initNewLight(light1, "Light 1", INFRAROUGE);

    Color *red = malloc(sizeof(Color));
    Color *green = malloc(sizeof(Color));
    Color *blue = malloc(sizeof(Color));
    *red = (Color){255, 0, 0};
    *green = (Color){0, 255, 0};
    *blue = (Color){0, 0, 255};

    add_color_to_light(light1, red);
    add_color_to_light(light1, green);
    add_color_to_light(light1, blue);

    Function *f1 = alloc_function();
    Function *f2 = alloc_function();
    set_funct(f1, ON);
    set_funct(f2, FADE1);

    add_function_to_light(light1, f1);
    add_function_to_light(light1, f2);

    insertFirst(l, light1, LIST_TYPE_LIGHT, compareLight);

    // Second Light
    Light *light2 = alloc_light();
    initNewLight(light2, "Light 2", INFRAROUGE);

    Color *yellow = malloc(sizeof(Color));
    Color *cyan = malloc(sizeof(Color));
    Color *magenta = malloc(sizeof(Color));
    *yellow = (Color){255, 255, 0};
    *cyan = (Color){0, 255, 255};
    *magenta = (Color){255, 0, 255};

    add_color_to_light(light2, yellow);
    add_color_to_light(light2, cyan);
    add_color_to_light(light2, magenta);

    Function *f3 = alloc_function();
    Function *f4 = alloc_function();
    set_funct(f3, OFF);
    set_funct(f4, BRIGHTNESS_MORE);

    add_function_to_light(light2, f3);
    add_function_to_light(light2, f4);

    insertFirst(l, light2, LIST_TYPE_LIGHT, compareLight);

    *state = l;
    return 0;
}


int setup_list_color(void **state) {
    List *l = allocList();
    initList(l,LIST_TYPE_COLOR);
    *state = l;
    return 0;
}

int teardown_list_light(void **state){
    List *list = (List *)(*state);
    if (list != NULL) {
        if (!isEmpty(list)) deleteList(list, deleteLight);
        else free(list);
    }
    return 0;
}

int teardown_list_color(void **state) {
    List *list = (List *)(*state);
    if (list != NULL) {
        if (!isEmpty(list)) deleteList(list, free_color);
        else free(list);
    }
    return 0;
}

// -------- GROUP 1: Init + Basic Checks --------

void test_initList(void **state) {
    List *list = (List *)(*state);
    assert_non_null(list);
    assert_ptr_equal(list->sentinel_begin.next, &list->sentinel_end);
    assert_ptr_equal(list->sentinel_end.previous, &list->sentinel_begin);
    assert_ptr_equal(list->current, &list->sentinel_begin);
}

void test_isEmpty_empty(void **state) {
    List *list = (List *)(*state);
    assert_true(isEmpty(list));
}

void test_isEmpty_notEmpty(void **state) {
    List *list = (List *)(*state);
    Light*light1 = alloc_light();
    initNewLight(light1,"Light 1",INFRAROUGE);
    insertFirst(list, light1, LIST_TYPE_LIGHT,compareLight);
    Light*light2 = alloc_light();
    initNewLight(light2, "Light 2", INFRAROUGE);
    insertFirst(list, light2, LIST_TYPE_LIGHT,compareLight);
    assert_false(isEmpty(list));
}

void test_isFirst_empty(void **state) {
    List *list = (List *)(*state);
    assert_false(isFirst(list));  // List is empty
}

void test_isFirst_1element(void **state) {
    List *list = (List *)(*state);

    setOnFirst(list);
    assert_true(isFirst(list));
    assert_false(isLast(list));

    setOnLast(list);
    assert_true(isLast(list));
    assert_false(isFirst(list));
}

void test_isLast_empty(void **state) {
    List *list = (List *)(*state);
    assert_false(isLast(list));  // Empty list
}

void test_isOutOfList_empty(void **state) {
    List *list = (List *)(*state);
    assert_false(isOutOfList(list));  // Empty list
}

void test_isOutOfList_notEmpty_in(void **state) {
    List *list = (List *)(*state);
    setOnFirst(list);
    assert_false(isOutOfList(list));
}

void test_setOnPrev_light(void **state){
    List *list = (List *)(*state);
    Light*light = alloc_light();
    initNewLight(light,"Light 1",INFRAROUGE);

    Color *red = alloc_color();
    set_color(red, 255, 0, 0);

    add_color_to_light(light, red);

    Function* f1 = alloc_function();
    Function* f2 = alloc_function();
    set_funct(f1,ON);
    set_funct(f2,FADE1);

    add_function_to_light(light, f1);
    add_function_to_light(light, f2);

    insertFirst(list, light,LIST_TYPE_LIGHT,compareLight);

    setOnFirst(list);
    setOnPrev(list);
    assert_false(isOutOfList(list));
}

void test_nbElement_light(void **state){
    List *list = (List *)(*state);
    assert_int_equal(nbElement(list),0);
    Light *light1 = alloc_light();
    Light *light2 = alloc_light();
    initNewLight(light1, "L1", INFRAROUGE);
    initNewLight(light2, "L2", INFRAROUGE);
    insertFirst(list, light1, LIST_TYPE_LIGHT, compareLight);
    insertFirst(list, light2, LIST_TYPE_LIGHT, compareLight);
    assert_int_equal(nbElement(list), 2);
}

void print_light_test(void *data){
    Light *l = (Light *)data;
    (void)l;
}

void test_printList_light(void **state){
    List *list = (List *)(*state);
    printList(list, print_light_test);
}

void test_find_light(void **state){
    List *list = (List *)(*state);
    setOnFirst(list);

    Light *query = alloc_light();
    initNewLight(query, "Test Light", INFRAROUGE);

    Color *red = alloc_color();
    set_color(red, 255, 0, 0);
    add_color_to_light(query, red);

    Function* f1 = alloc_function();
    Function* f2 = alloc_function();
    set_funct(f1,ON);
    set_funct(f2,FADE2);

    add_function_to_light(query, f1);
    add_function_to_light(query, f2);

    insertFirst(list, query,LIST_TYPE_LIGHT,compareLight);

    assert_int_equal(find_exact(list, query, compareLight),1); //found
    assert_ptr_equal(getCurrentData(list), list->current->data);
}

void test_find_min_light(void **state) {
    List *list = (List *)(*state);

    // Traverse from sentinel
    NodeList *firstNode = list->sentinel_begin.next;
    Light *light1 = (Light *)firstNode->data;
    Light *light2 = (Light *)firstNode->next->data;

    void *found = NULL;

    // Search for minimum using light2 as reference
    int res = find_min(list, light2, &found, compareLight);

    assert_true(res < INT_MAX);

    if (compareLight(light1, light2) < 0) {
        assert_ptr_equal(found, light1);
    } else {
        assert_ptr_not_equal(found, light2);
    }
}

void test_getCurrentData_null(void **state){
    assert_null(getCurrentData(NULL));
}

void test_setOnFirst_empty(void **state){
    List *list = (List *)(*state);
    setOnFirst(list);
    assert_true(isEmpty(list));
}

void test_insertFirst_wrong_type(void **state){
    List *list = (List *)(*state);

    Color* red = alloc_color();
    set_color(red, 255, 0, 0);

    assert_int_equal(insertFirst(list, red, LIST_TYPE_LIGHT, compareLight), 0);
    free(red);
}

void test_deleteValue_not_found(void **state){
    List *list = (List *)(*state);
    Color *c1 = alloc_color();
    Color *c2 = alloc_color();
    set_color(c1, 255, 0, 0);
    set_color(c2, 0, 255, 0);
    insertFirst(list, c1, LIST_TYPE_COLOR, compareColorExact);
    insertFirst(list, c2, LIST_TYPE_COLOR, compareColorExact);

    // Couleur non présente (bleu)
    Color *not_in_list = alloc_color();
    set_color(not_in_list, 0, 0, 255);

    // On tente de la supprimer (ne devrait rien supprimer)
    assert_false(deleteValue(list, not_in_list, compareColorExact, free_color));

    // Vérifie que les deux couleurs initiales sont toujours là
    assert_int_equal(nbElement(list), 2);

    // Nettoyage
    free_color(not_in_list);
}

void test_deleteValue_found(void **state) {
    List *list = (List *)(*state);

    // Allocate and insert two colors
    Color *red = alloc_color();
    Color *green = alloc_color();
    set_color(red, 255, 0, 0);
    set_color(green, 0, 255, 0);

    insertFirst(list, red, LIST_TYPE_COLOR, compareColorExact);
    insertFirst(list, green, LIST_TYPE_COLOR, compareColorExact);

    // Delete the red color (which is now the second node)
    assert_true(deleteValue(list, red, compareColorExact, free_color));

    // Only green should remain
    assert_int_equal(nbElement(list), 1);

    // Check that green is still present
    setOnFirst(list);
    Color *remaining = getCurrentData(list);
    assert_true(compareColorExact(green, remaining));

    // Clean up: free green (red is already freed by deleteValue)
    // (teardown will handle this)
}

void test_initList_null(void **state) {
    assert_int_equal(initList(NULL, LIST_TYPE_COLOR),-1);
}

void test_isOutOfList_out(void **state) {
    assert_true(isOutOfList(NULL));
}

void test_insertFirst_notFound(void **state) {
    List *list = (List *)(*state);

    Color *red = alloc_color();
    set_color(red, 255, 0, 0);
    insertFirst(list, red, LIST_TYPE_COLOR, compareColorExact);

    Color *not_in_list = alloc_color();
    set_color(not_in_list, 0, 255, 0);

    assert_false(find_exact(list, not_in_list, compareColorExact));

    // Clean up
    free_color(not_in_list);
}

void test_find_min_null(void **state) {
    List *list = (List *)(*state);

    Color *red = alloc_color();
    set_color(red, 255, 0, 0);
    insertFirst(list, red, LIST_TYPE_COLOR, compareColorExact);

    void *found = NULL;

    assert_int_equal(find_min(NULL, red, &found, compareColorExact), 0);
    assert_int_equal(find_min(list, red, &found, NULL), 0);
    assert_int_equal(find_min(list, NULL, &found, compareColorExact), 0);
}

void test_deleteValue_null(void **state) {
    List *list = (List *)(*state);

    Color *red = alloc_color();
    set_color(red, 255, 0, 0);
    insertFirst(list, red, LIST_TYPE_COLOR, compareColorExact);
    assert_false(deleteValue(NULL, red, compareColorExact, free_color));
    assert_int_equal(deleteValue(NULL, red, compareColorExact, free_color),0);
    assert_false(deleteValue(list, NULL, compareColorExact, free_color));
    assert_false(deleteValue(list, red, NULL, free_color));
}

void test_insertFirst_duplicate(void **state) {
    List *list = (List *)(*state);

    // Allocate and initialize a color
    Color *red1 = alloc_color();
    set_color(red1, 255, 0, 0);

    // Insert first instance - should succeed
    int ret1 = insertFirst(list, red1, LIST_TYPE_COLOR, compareColorExact);
    assert_int_equal(ret1, 1);
    assert_int_equal(nbElement(list), 1);

    // Allocate a second color with the same values (duplicate)
    Color *red2 = alloc_color();
    set_color(red2, 255, 0, 0);

    // Attempt to insert duplicate - should fail
    int ret2 = insertFirst(list, red2, LIST_TYPE_COLOR, compareColorExact);
    assert_int_equal(ret2, 0);
    assert_int_equal(nbElement(list), 1);  // List size should remain 1

    // Clean up
    free_color(red2);
}
