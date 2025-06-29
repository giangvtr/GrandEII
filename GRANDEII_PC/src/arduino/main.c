//
// Created by boris on 4/21/25.
//

#include "communication.h"
#include "arduino_utils.h"

ArduinoController* myArduino ;

int main(){

  printf("This is a test\n");
  update_arduino_list();

  myArduino= create_arduino_controller();
  myArduino->port = "/dev/ttyACM0";
  open_port(myArduino);
  //read_arduino(myArduino);
  unsigned char test[IR_CODE_LENGTH] = {0x7F, 0x80, 0x7F, 0x40};

  write_arduino(myArduino, test);
  return 0;
}