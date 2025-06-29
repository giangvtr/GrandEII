
#include "light.h"

static uint32_t light_id_counter = 0;

Light* alloc_light() {
    Light *light = malloc(sizeof(Light));
    if (light == NULL) {
        fprintf(stderr, "Error creating new light\n");
        return NULL;
    }

    light->color_list = malloc(sizeof(List));
    if (light->color_list == NULL) {
        free(light);
        fprintf(stderr, "Error creating color list\n");
        return NULL;
    }

    light->function_list = malloc(sizeof(List));
    if (light->function_list == NULL) {
        free(light->color_list);
        free(light);
        fprintf(stderr, "Error creating function list\n");
        return NULL;
    }

    light->id = -1;
    return light;
}

int initNewLight(Light* light, const char* name, technologyLight technology) {
    if (light == NULL) {
        fprintf(stderr, "Light not yet allocated");
        return -1;
    }
    // Copy the name into the fixed-length array
    strncpy(light->name, name, MAX_NAME_LENGTH - 1);
    light->name[MAX_NAME_LENGTH - 1] = '\0';  // Ensure null-termination

    // Assign default color (do not free it later)
    light->color = NULL;

    // Set defaults if technology or port are not provided
    light->technology = (technology == INFRAROUGE) ? INFRAROUGE : technology;
    light->brightness = 255;  // Default brightness
    light->id = light_id_counter++;

    initList(light->color_list, LIST_TYPE_COLOR);
    initList(light->function_list, LIST_TYPE_FUNCTION);

    return 0;
}

int setColor(Light* light, Color* color) {
    if (light == NULL | color == NULL) return -1;
    if (find_exact(light->color_list, color, compareColorExact) == 1) {
        light->color = color;
        return 0;
    }
    return -1;
}

int setBrightness(Light* light, int brightness) {
    if (light == NULL) {
        return -1;
    }
    if (brightness >= 0 && brightness <= 255) {
        light->brightness = brightness;
        return 0;
    }
    printf("Invalid brightness value. It should be between 0 and 255.\n");
    return -1;
}

int setLightName(Light* light, const char* name) {
    if (light == NULL) {
        return -1;
    }
    if (name != NULL) {
        strncpy(light->name, name, MAX_NAME_LENGTH - 1);
        return 0;
    }
    return -1;
}

uint32_t getLightId(const Light *light) {
    if (light==NULL) return 255;
    return light->id;
}

const char* getLightName(const Light *light) {
    if (light==NULL) return NULL;
    return light->name;
}

int printLight(Light* light) {
    if (light == NULL) {
        fprintf(stderr, "The light does not exist\n");  // Fixed stder to stderr
        return -1;
    }
    printf("\tLight Name: %s\n", light->name);
    printf("\tBrightness: %d\n", light->brightness);
    printf("\tTechnology: %d\n", light->technology);  // Prints the enum value

    // Check if light->color is NULL before accessing its fields
    if (light->color == NULL) {
        printf("\tColor is not set.\n");
    }
    printf("\tLight ID: %u\n", light->id);
    if (nbElement(light->color_list)!=0)printList(light->color_list, print_color_info);
    if (nbElement(light->function_list)!=0)printList(light->function_list, print_function_info);

    return 0;
}

int compareLight( void* a,  void* b) {
    const Light* light1 = (const Light*)a;
    const Light* light2 = (const Light*)b;

    if (light1 == NULL || light2 == NULL) return 0;

    //Case light NULL, id < 0
    if (light1->id < 0 || light2->id < 0) return 0;
    //Case 2 ids differs
    if (light1->id != light2->id) return 0;
    return 1;
}

Color* getNearestColor(Light* light, Color* color_to_compare){
    if (light==NULL || color_to_compare == NULL) return NULL;
    if (isEmpty(light->color_list)) return NULL;
    List* color_list = light->color_list;
    Color* nearest_color = NULL;
    int nearest_color_dist = INT_MAX;
    int nb_color = nbElement(color_list);
    setOnFirst(color_list);
    for (int i=0; i<nb_color;i++){
        int temp = compareColorDist(color_list->current->data, color_to_compare);
        if (temp < nearest_color_dist){
            nearest_color_dist = temp;
            nearest_color = color_list->current->data;
        }
        setOnNext(color_list);
    }
    return nearest_color;
}

Function* getFunction(Light* light, FunctionName function_to_compare){
    if (light==NULL) return NULL;

    //Retrieve the function list
    List* function_list = light->function_list;
    int nb_funct = nbElement(function_list);
    setOnFirst(function_list);

    //Function fund
    Function* function = NULL;

    //Cycle through each of the functions
    for (int i=0; i<nb_funct;i++){
        function = function_list->current->data;
        if (function->name==function_to_compare){
            return function;
        }
        setOnNext(function_list);
    }
    //Nothing found
    return NULL;
}

void deleteLight(void* data) {
    if (data == NULL) return;

    Light* light = (Light*)data;

    if (light->color_list != NULL) {
        if (!isEmpty(light->color_list)) {
            deleteList(light->color_list, free_color);
        } else {
            // Liste vide mais allouée → on la libère quand même
            free(light->color_list);
        }
    }

    if (light->function_list != NULL) {
        if (!isEmpty(light->function_list)) {
            deleteList(light->function_list, free_function);
        } else {
            free(light->function_list);
        }
    }
    free(light);
}


int add_color_to_light(Light* light, Color* color) {
    if (light == NULL || color == NULL) return -1;
    insertFirst(light->color_list, color, LIST_TYPE_COLOR, compareColorExact);
    return 0;
}

int add_function_to_light(Light* light, Function * function_to_add) {
    if (light ==NULL) return -1;
    insertFirst(light->function_list, function_to_add, LIST_TYPE_FUNCTION, compareFunction);
    return 0;
}

