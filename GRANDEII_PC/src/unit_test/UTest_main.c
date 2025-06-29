#include "UTest-color.h"
#include "UTest-light.h"
#include "UTest-list.h"
#include "UTest-function.h"
#include "UTest-group.h"
#include "UTest-gmanager.h"

/**
 * @brief Main function to execute all unit tests
 *
 * Runs the test suite using CMocka framework.
 *
 * @return Exit status based on test results
 */
int main_units_tests(void) {
    const struct CMUnitTest tests[] = {
        //Tests color
       cmocka_unit_test_setup_teardown(test_init_color, setup_color, teardown_color),
       cmocka_unit_test(test_init_color_NULL),
       cmocka_unit_test_setup_teardown(test_print_color_info, setup_color, teardown_color),
       cmocka_unit_test(test_print_color_info_NULL),
       cmocka_unit_test(test_rgbToHsl_basic_colors),
       cmocka_unit_test(test_compareColorExact_equal_colors),
       cmocka_unit_test(test_compareColorExact_equal_colors2),
       cmocka_unit_test(test_compareColorExact_different_colors),
       cmocka_unit_test(test_compareColorExact_null_input),
       cmocka_unit_test(test_compareColorDist_typical_colors),
       cmocka_unit_test(test_compareColorDist_null_input),

       // //  //Tests light
       cmocka_unit_test_setup_teardown(test_initNewLight, setup_light, teardown_light),
       cmocka_unit_test(test_initNewLight_NULL),
       cmocka_unit_test_setup_teardown(test_setBrightness_valid, setup_light, teardown_light),
       cmocka_unit_test_setup_teardown(test_setBrightness_invalid, setup_light, teardown_light),
       cmocka_unit_test_setup_teardown(test_deleteLight_null, NULL, NULL),
       cmocka_unit_test_setup_teardown(test_printLight, setup_light, teardown_light),
        cmocka_unit_test_setup_teardown(test_setColor,setup_light,teardown_light),
        cmocka_unit_test_setup_teardown(test_setColor_colorNotInList,setup_light,teardown_light),

       cmocka_unit_test(test_printLight_lightNull),
       cmocka_unit_test_setup_teardown(test_setLightName_null, setup_light, teardown_light),
       cmocka_unit_test_setup_teardown(test_setLightName_valid, setup_light, teardown_light),
       cmocka_unit_test_setup_teardown(test_getLightId, setup_light, teardown_light),
       cmocka_unit_test(test_getLightId_NULL),
       cmocka_unit_test_setup_teardown(test_getLightName, setup_light, teardown_light),
       cmocka_unit_test(test_getLightName_NULL),
       cmocka_unit_test(test_setBrightness_NULL),
       cmocka_unit_test_setup_teardown(test_compareLight_null_inputs, setup_light, teardown_light),
       cmocka_unit_test(test_compareLight_negative_id),
       cmocka_unit_test(test_compareLight_different_id),
       cmocka_unit_test(test_compareLight_same_id),
       //
       cmocka_unit_test(test_getNearestColor_light_null),
       cmocka_unit_test_setup_teardown(test_getNearestColor_color_null,setup_light, teardown_light),
       cmocka_unit_test_setup_teardown(test_getNearestColor_empty_list,setup_empty_light, teardown_light),
       cmocka_unit_test_setup_teardown(test_getNearestColor_finds_nearest, setup_light, teardown_light),
       //
       cmocka_unit_test(test_getFunction_light_null),
       cmocka_unit_test_setup_teardown(test_getFunction_empty_list, setup_light, teardown_light),
       cmocka_unit_test_setup_teardown(test_getFunction_finds_function,setup_light,teardown_light),

      	// // // --------Test list - GROUP 1: Init + Basic Checks --------
        cmocka_unit_test_setup_teardown(test_initList, setup_list_light, teardown_list_light),
        cmocka_unit_test(test_initList_null),

      cmocka_unit_test_setup_teardown(test_isEmpty_empty, setup_list_light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_isEmpty_notEmpty, setup_list_light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_isFirst_empty, setup_list_light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_isFirst_1element, setup_list_light_2light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_isLast_empty, setup_list_light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_isOutOfList_empty, setup_list_light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_isOutOfList_notEmpty_in, setup_list_light_2light, teardown_list_light),
        cmocka_unit_test(test_isOutOfList_out),

      cmocka_unit_test_setup_teardown(test_setOnPrev_light, setup_list_light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_nbElement_light, setup_list_light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_printList_light, setup_list_light_2light, teardown_list_light),
        cmocka_unit_test_setup_teardown(test_printList_light, setup_list_color, teardown_list_color),
      cmocka_unit_test_setup_teardown(test_find_light, setup_list_light_2light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_find_min_light, setup_list_light_2light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_find_min_null, setup_list_color, teardown_list_color),

      cmocka_unit_test(test_getCurrentData_null),
      cmocka_unit_test_setup_teardown(test_setOnFirst_empty, setup_list_light, teardown_list_light),
      cmocka_unit_test_setup_teardown(test_insertFirst_wrong_type, setup_list_color, teardown_list_color),
        cmocka_unit_test_setup_teardown(test_insertFirst_notFound,setup_list_color,teardown_list_color),
      cmocka_unit_test_setup_teardown(test_insertFirst_duplicate, setup_list_color, teardown_list_color),
      cmocka_unit_test_setup_teardown(test_deleteValue_not_found, setup_list_color, teardown_list_color),
        cmocka_unit_test_setup_teardown(test_deleteValue_found,setup_list_color, teardown_list_color),
      cmocka_unit_test_setup_teardown(test_deleteValue_null, setup_list_color,teardown_list_color),

       //  //=========== TEST FUNCTION ==============
       cmocka_unit_test(test_alloc_function),
       cmocka_unit_test_setup_teardown(test_set_funct_valid, setup_function, teardown_function),
       cmocka_unit_test(test_set_funct_null),
       cmocka_unit_test_setup_teardown(test_print_function_info, setup_function, teardown_function),
       cmocka_unit_test(test_print_function_info_null),
       cmocka_unit_test(test_compareFunction_equal),
       cmocka_unit_test(test_compareFunction_not_equal),
       cmocka_unit_test_setup_teardown(test_compareFunction_null, setup_function, teardown_function),

        //========== TEST GROUP ===============
        cmocka_unit_test_setup_teardown(test_initGroup_valid, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_initGroup_null, setup_group,teardown_group),
        cmocka_unit_test_setup_teardown(test_addLighttoGroup_valid, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_addLighttoGroup_null, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_addLighttoGroup_invalid, setup_group, teardown_group),
      cmocka_unit_test_setup_teardown(test_addLighttoGroup_insertFirst_fail, setup_group, teardown_group),

        cmocka_unit_test_setup_teardown(test_isEmptyGroup_empty, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_isEmptyGroup_notEmpty,setup_group, teardown_group),

        cmocka_unit_test_setup_teardown(test_setGroupBrightness, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_setGroupColor, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_getGroupName, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_getGroupID, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_getGroupColor, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_compareGroup_equal, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_compareGroup_not_equal, setup_group, teardown_group),
      cmocka_unit_test(test_compareGroup_name_null),
        cmocka_unit_test_setup_teardown(test_printGroup, setup_group, teardown_group),
        cmocka_unit_test_setup_teardown(test_printGroup_color_none, setup_group,teardown_group),
      cmocka_unit_test_setup_teardown(test_printGroup_lights_present,setup_group, teardown_group),
        cmocka_unit_test(test_freeGroup_null),

      //======== test group manager
        cmocka_unit_test(test_createGroupManager_valid),
        cmocka_unit_test(test_createGroupManager_invalid),
        cmocka_unit_test_setup_teardown(test_addEmptyGroup_success, setup_manager, teardown_manager),
        cmocka_unit_test_setup_teardown(test_addEmptyGroup_failure, setup_manager, teardown_manager),
        cmocka_unit_test_setup_teardown(test_assignGroupToManager_success, setup_manager, teardown_manager),
        cmocka_unit_test_setup_teardown(test_assignGroupToManager_failure, setup_manager_full, teardown_manager),
        cmocka_unit_test_setup_teardown(test_removeGroup_success, setup_manager, teardown_manager),
        cmocka_unit_test_setup_teardown(test_removeGroup_failure, setup_manager_full, teardown_manager),
        cmocka_unit_test_setup_teardown(test_printGroupManager, setup_manager_full, teardown_manager),
        cmocka_unit_test_setup_teardown(test_printGroupManager_no_groups, setup_manager, teardown_manager),
      cmocka_unit_test_setup_teardown(test_printGroupManager, setup_manager, teardown_manager),
        cmocka_unit_test_setup_teardown(test_deleteGroupManager, setup_manager, teardown_manager),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}