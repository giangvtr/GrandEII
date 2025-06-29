//
// Created by boris on 3/3/25.
//

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#define START_CHAR 0x02
#define STOP_CHAR 0x03
#define IR_CODE_LENGTH 4 ///32bit value in an array of char
#define DEBUG_COMM 1

/**
 * @brief Struct for managing arduino serial interface
 */
typedef struct {
    char* port; /**< Serial string of the arduino port */
    int arduino_ID; /**<ID of the arduino*/
    struct termios tty; /**< Parameter of the tty link*/
    char isOpened;   /**< Is the Arduino Port opened*/
    unsigned char ir_last_code[IR_CODE_LENGTH]; /**<Last code received by the arduino when read_arduino is called*/
} ArduinoController;

/**
 * @brief Create and allocatte an ArduinoController type
 * @return Pointer to the ArduinoController
 */
ArduinoController* create_arduino_controller();
/**
 * @brief Free the allocation of the AruinoController and closes the port
 * @param arduino the ArduinoController pointer
 * @return 0 if success, -1 if arduino is NULL
 */
int free_arduino_controller(ArduinoController* arduino_controller);
/**
 * @brief Open a port to comunicate with an Arduino
 * @param port String of the port, usually, "/dev/ttyUSB0"
 * @warning Baud is 115200 and should be the same with the Arduino on the other side!
 * @return 0 if success, any other value if not: TODO:Error codes
 */
int open_port(ArduinoController* arduino);
/**
 * @brief Closes the arduino port
 * @param arduino AruinoController
 * @return 0 if success, -1 if arduino is NULL, -2 if port is already closed
 */
int close_port(ArduinoController* arduino);
/**
 * @brief Read IR codes from arduino and update ir_last_code array
 * @param arduino Arduino controller
 * @return 0 if successful, failure if anything else
 * @warning this function is blocking
 */
int read_arduino(ArduinoController* arduino);
/**
 * @brief Write IR data to the arduino
 * @param arduino Pointer to the ArduinoController
 * @param data Array of IR data with size of IR_CODE_LENGTH
 * @return 0 if successful
 */
int write_arduino(ArduinoController* arduino, unsigned char* data);
/**
 * @brief Checks if the arduino is well connected
 * @param arduino Pointer to the ArduinoController and is valid
 * @return 0 if the Arduino is connected and non-null; -1 else
 */
int arduino_valid(ArduinoController* arduino);

#endif //COMMUNICATION_H
