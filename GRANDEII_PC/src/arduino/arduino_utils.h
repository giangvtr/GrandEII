//
// Created by boris on 3/24/25.
//

#ifndef ARDUINO_UTILS_H
#define ARDUINO_UTILS_H

#define MAX_ARDUINO 16
#define MAX_CHAR_ARDUINO_ID 50

//Global variables
extern char arduinoList[MAX_ARDUINO][2][MAX_CHAR_ARDUINO_ID];
extern char arduinoList_length;

int update_arduino_list();
int IR_Code_to_INT(const unsigned char *ir_code);
void INT_to_IR_Code(int formattedIRCode, unsigned char *ir_code);
#endif //ARDUINO_UTILS_H
