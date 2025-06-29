//
// Created by boris on 3/24/25.
//

#include "arduino_utils.h"

#include <stdio.h>
#include <string.h>
/**
 * @brief Triple array list. containing all the arduinos, their name and path
 * @see update_arduino_list
 */
char arduinoList[MAX_ARDUINO][2][MAX_CHAR_ARDUINO_ID];
char arduinoList_length = 0;

int update_arduino_list(){
    /** Script to enumerate all arduino ports:
     * first line: 1st arduino ID
     * second line: 1st arduino tty path
     * third line: 2nd arduino ID
     * fourth line: 2nd arduino tty path
     * etc...

        #!bin/bash
        cd /dev/serial/by-id
        for id in $(ls /dev/serial/by-id/); do
          FULLPATH=$(realpath $id);
          ONLYID=$(echo $id | grep -E "(Arduino_.*_)" -o);
          echo "$ONLYID\n$FULLPATH";
        done

     */
    FILE *cmd=popen("cd /dev/serial/by-id 2> /dev/null && for id in $(ls /dev/serial/by-id/); do FULLPATH=$(realpath $id);ONLYID=$(echo $id | grep -E \"(Arduino_.*_)\" -o);echo \"$ONLYID\n$FULLPATH\";done", "r");
    //Parse results
    if (cmd==(FILE*) NULL) return -1;
    char result[MAX_CHAR_ARDUINO_ID]={0x0};
    int i = 0;
    while (fgets(result, sizeof(result), cmd) !=NULL){
        memcpy(arduinoList[i/2][i%2], result, MAX_CHAR_ARDUINO_ID);
        i++;
    }
    arduinoList_length = i/2;
    pclose(cmd);
    return arduinoList_length;
}

int IR_Code_to_INT(const unsigned char *ir_code){
    int formattedIRcCode = 0;
    formattedIRcCode |= ir_code[3];
    formattedIRcCode |= ir_code[2]<<8;
    formattedIRcCode |= ir_code[1]<<16;
    formattedIRcCode |= ir_code[0]<<24;
    return formattedIRcCode;
}
void INT_to_IR_Code(int formattedIRCode, unsigned char* ir_code){
    ir_code[0] = formattedIRCode&0xFF;
    ir_code[1] = (formattedIRCode&0xFF00)>>8;
    ir_code[2] = (formattedIRCode&0xFF0000)>>16;
    ir_code[3] = (formattedIRCode&0xFF000000)>>24;
}