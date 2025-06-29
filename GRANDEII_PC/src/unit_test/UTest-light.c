//
// Created by giangvu on 3/15/25.
//

#include "UTest-light.h"
#include "../model/light.h"

// --- Setup and Teardown functions ---
int setup_light(void **state) {
    Light *light = alloc_light();
    initNewLight(light,"Test light",INFRAROUGE);

    Color *red = malloc(sizeof(Color));
    Color *green = malloc(sizeof(Color));
    Color *blue = malloc(sizeof(Color));
    *red = (Color){255, 0, 0};
    *green = (Color){0, 255, 0};
    *blue = (Color){0, 0, 255};

    add_color_to_light(light, red);
    add_color_to_light(light, green);
    add_color_to_light(light, blue);

    Function* f1 = alloc_function();
    Function* f2 = alloc_function();
    set_funct(f1,ON);
    set_funct(f2,FADE1);

    add_function_to_light(light, f1);
    add_function_to_light(light, f2);
    *state = light; // Store the allocated light in the state
    return 0;
}

int setup_empty_light(void **state) {
    Light *light = alloc_light();
    initNewLight(light, "Empty Light", INFRAROUGE);
    *state = light;
    return 0;
}

int teardown_light(void **state) {
    Light *light = (Light*)*state;
    if (light != NULL) {
        deleteLight(light); // Free lists inside light
    }
    return 0;
}

// --- Test functions ---

void test_initNewLight(void **state) {
    Light *light = (Light*)*state; // Retrieve the light from the state
    assert_string_equal(light->name, "Test light");
    assert_int_equal(light->technology, INFRAROUGE);
    assert_int_equal(light->brightness, 255);
    assert_null(light->color);
}

void test_initNewLight_NULL(void **state) {
    (void)state;
    assert_int_equal(initNewLight(NULL, "Test Light", INFRAROUGE), -1);
}



void test_setBrightness_valid(void **state) {
    Light *light = (Light*)*state;
    setBrightness(light, 150);
    assert_int_equal(light->brightness, 150);
}

void test_setBrightness_invalid(void **state) {
    Light *light = (Light*)*state;
    setBrightness(light, 300); // Try to set an invalid brightness
    assert_int_equal(light->brightness, 255); // Brightness should remain unchanged
}

void test_deleteLight_null(void **state) {
    (void)state; // unused variable
    // Call deleteLight with a NULL pointer and ensure it doesn't crash
    deleteLight(NULL);
    assert_true(1); // If code continue, the exception is ignored.
}

void test_printLight(void **state) {
    Light *light = (Light*)*state;

    // Redirect the stdout to a buffer to capture printed output
    FILE *orig_stdout = stdout;
    char buffer[1024];
    stdout = fmemopen(buffer, sizeof(buffer), "w");

    // Call the function being tested
    printLight(light);
    // Check if the output is correct
    fflush(stdout);
    stdout = orig_stdout; // Restore stdout

	    // Expected output format
    const char *expected_output = "\tLight Name: Test light\n"
                                  "\tBrightness: 255\n"
                                  "\tTechnology: 1\n"
                                  "\tColor is not set.\n"
                                  "\tLight ID: 3\n"
                                  "Color: R=0, G=0, B=255\n"
                                  "Color: R=0, G=255, B=0\n"
                                  "Color: R=255, G=0, B=0\n"
                                  "\t\tFunction name: 3\n"
                                  "\t\tFunction name: 1\n";
    assert_string_equal(buffer, expected_output);
}

void test_printLight_lightNull(void **state) {
    (void)state;
    assert_int_equal(printLight(NULL),-1);
}

void test_setLightName_null(void **state) {
    Light *light = (Light*)*state;
    // NULL light
    assert_int_equal(setLightName(NULL, "ValidName"), -1);
    // NULL name
    assert_int_equal(setLightName(light, NULL), -1);
}

void test_setLightName_valid(void **state) {
    Light *light = (Light*)*state;
    assert_int_equal(setLightName(light, "NewName"), 0);
    assert_string_equal(light->name, "NewName");
}

void test_getLightId(void **state) {
    Light *light = (Light*)*state;
    assert_int_equal(getLightId(light), light->id);
}

void test_getLightId_NULL(void **state) {
    assert_int_equal(getLightId(NULL), 255);
}

void test_getLightName(void **state) {
    Light *light = (Light*)*state;
    assert_string_equal(getLightName(light), light->name);
}

void test_getLightName_NULL(void **state) {
    assert_null(getLightName(NULL));
}

void test_setBrightness_NULL(void **state) {
    assert_int_equal(setBrightness(NULL, 100), -1);
}

void test_compareLight_null_inputs(void **state) {
    Light *light = (Light*)*state;
    assert_int_equal(compareLight(NULL, light), 0);
    assert_int_equal(compareLight(light, NULL), 0);
    assert_int_equal(compareLight(NULL, NULL), 0);
}

void test_compareLight_negative_id(void **state) {
    Light *l1 = alloc_light();
    Light *l2 = alloc_light();
    initNewLight(l1, "Test Light 1", INFRAROUGE);
    initNewLight(l2, "Test Light 2", INFRAROUGE);
    assert_int_equal(compareLight(l1, l2), 0);
    l1->id = 2;
    l2->id = 10;
    assert_int_equal(compareLight(l1, l2), 0);
    deleteLight(l1);
    deleteLight(l2);
}

void test_compareLight_different_id(void **state) {
    Light *l1 = alloc_light();
    Light *l2 = alloc_light();
    initNewLight(l1, "Test Light", INFRAROUGE);
    initNewLight(l2, "Test Light", INFRAROUGE);
    l1->id = 1;
    l2->id = 2;
    assert_int_equal(compareLight(l1, l2), 0);
    deleteLight(l1);
    deleteLight(l2);

}

void test_compareLight_same_id(void **state) {
    Light *l1 = alloc_light();
    Light *l2 = alloc_light();
    initNewLight(l1, "Test Light", INFRAROUGE);
    initNewLight(l2, "Test Light", INFRAROUGE);
    l1->id = 42;
    l2->id = 42;
    assert_int_equal(compareLight(l1, l2), 1);
    deleteLight(l1);
    deleteLight(l2);

}

void test_getNearestColor_light_null(void **state) {
    Color dummy = {1,2,3};
    assert_null(getNearestColor(NULL, &dummy));
}

void test_getNearestColor_color_null(void **state) {
    Light*light = (Light*)*state;
    assert_null(getNearestColor(light, NULL));
}

void test_getNearestColor_empty_list(void **state) {
    Light*light = (Light*)*state;
    Color dummy = {50,200,37};
    assert_null(getNearestColor(light, &dummy));
}

void test_getNearestColor_finds_nearest(void **state) {
    Light*light = (Light*)*state;

    Color to_compare = {254, 0, 0}; // Closest to red
    Color *nearest = getNearestColor(light, &to_compare);
    assert_non_null(nearest);
    assert_int_equal(nearest->r, 255);
    assert_int_equal(nearest->g, 0);
    assert_int_equal(nearest->b, 0);

}

void test_getFunction_light_null(void **state) {
    assert_null(getFunction(NULL, 0));
}

void test_getFunction_empty_list(void **state) {
    Light*light = (Light*)*state;

    assert_null(getFunction(light, BRIGHTNESS_LESS));
}

void test_getFunction_finds_function(void **state) {
    Light*light = (Light*)*state;
    Function* f1 = alloc_function();
    Function* f2 = alloc_function();
    set_funct(f1, OFF);
    set_funct(f2, JUMP);
    add_function_to_light(light, f1);
    add_function_to_light(light, f2);

    assert_ptr_equal(getFunction(light, OFF), f1);
    assert_ptr_equal(getFunction(light, JUMP), f2);
    assert_null(getFunction(light, 99)); // Not found
}

void test_setColor(void **state) {
    Light* light = (Light*)*state;

    Color* color = malloc(sizeof(Color));
    color->r = 100;
    color->g = 150;
    color->b = 200;

    // Ajouter la couleur à la liste pour que find_exact puisse la trouver
    add_color_to_light(light, color);

    assert_int_equal(setColor(light, color), 0);
    assert_non_null(light->color);
    assert_int_equal(light->color->r, 100);
    assert_int_equal(light->color->g, 150);
    assert_int_equal(light->color->b, 200);
}

void test_setColor_colorNotInList(void **state) {
    Light* light = (Light*)*state;

    Color* color = malloc(sizeof(Color));
    color->r = 255;
    color->g = 255;
    color->b = 0;

    // Ne PAS ajouter la couleur à light->color_list

    // Donc find_exact échouera => setColor retourne -1
    assert_int_equal(setColor(light, color), -1);

    // Vérifie que la couleur n’a pas été assignée
    assert_true(light->color == NULL ||
                light->color->r != 255 ||
                light->color->g != 255 ||
                light->color->b != 0);

    free(color);
}
