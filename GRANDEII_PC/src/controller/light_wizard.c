//
// Created by boris on 4/24/25.
//

#include "light_wizard.h"
#include "../arduino/communication.h"

static int consecutives_retries;





/**
 * @brief Matches a color structure with the last IR code received from the Arduino.
 *
 * This function reads the most recent IR code detected by the Arduino and copies it
 * into the provided `Color` structure. It first verifies the validity of both the
 * `Color` and `ArduinoController` pointers before proceeding.
 *
 * @param color A pointer to the Color structure that will receive the IR code.
 * @param arduino A pointer to the ArduinoController structure used to read the IR signal.
 * @return 0 on success, -1 if either pointer is NULL or the Arduino is not valid.
 *
 * @note This function assumes the IR code has already been captured by the Arduino
 * and stored in `arduino->ir_last_code`.
 *
 * @see read_arduino, arduino_valid
 */
int match_color_with_IR(Color *color, ArduinoController *arduino) {
  //Test if valid
  if(color==(Color *)NULL) return -1;
  if(!arduino_valid(arduino)) return -1;

  //Waiting for IR
  read_arduino(arduino);
  memcpy(color->ir_code, arduino->ir_last_code, IR_CODE_LENGTH);//copy the code

  return 0;
}