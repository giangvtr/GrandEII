//
// Created by boris on 3/3/25.
//

#include "communication.h"
#include <stdlib.h>

const char START_N_STOP_BUF[2] = {START_CHAR, STOP_CHAR};

ArduinoController* create_arduino_controller(){
  ArduinoController* created_arduino = malloc(sizeof(ArduinoController));
  memset(&created_arduino->tty, 0, sizeof(struct termios));
  created_arduino->isOpened=0;
  created_arduino->port = "/dev/ttyACM0";
  return created_arduino;
}

int free_arduino_controller(ArduinoController* arduino){
  if (arduino==(ArduinoController*)NULL) return -1;
  close_port(arduino);
  free(arduino);
}

int open_port(ArduinoController* arduino){
  if (arduino==(ArduinoController*)NULL) return -1;

  //Open port
  arduino->arduino_ID = open(arduino->port, O_RDWR);
  if (arduino->arduino_ID == -1) return -1;
  if(DEBUG_COMM) printf("Opening %s and Arduino file handler %d\n", arduino->port, arduino->arduino_ID);


  // Read in existing settings, and handle any error
  if(tcgetattr(arduino->arduino_ID, &arduino->tty) != 0) {
    if(DEBUG_COMM) printf("%s: Error %i from tcgetattr: %s\n", arduino->port, errno, strerror(errno));
    return 1;
  }


  //Flags...
  arduino->tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  arduino->tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  arduino->tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
  arduino->tty.c_cflag |= CS8; // 8 bits per byte (most common)
  arduino->tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  arduino->tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  arduino->tty.c_lflag &= ~ICANON;
  arduino->tty.c_lflag &= ~ECHO; // Disable echo
  arduino->tty.c_lflag &= ~ECHOE; // Disable erasure
  arduino->tty.c_lflag &= ~ECHONL; // Disable new-line echo
  arduino->tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  arduino->tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  arduino->tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  arduino->tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  arduino->tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  arduino->tty.c_cc[VTIME] = 0;    //Wait time is inf
  arduino->tty.c_cc[VMIN] = 6;     //Wait until 6 char in intput

  //Set speed 115200 baud
  cfsetispeed(&arduino->tty, B115200);
  cfsetospeed(&arduino->tty, B115200);

  // Save tty settings, also checking for error
  if (tcsetattr(arduino->arduino_ID, TCSANOW, &arduino->tty) != 0) {
    if(DEBUG_COMM) printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    return 1;
  }
  arduino->isOpened = 1;
  if (DEBUG_COMM) printf("Successfully opened %s\n", arduino->port);
  return 0;
}

int close_port(ArduinoController* arduino){
  if (arduino==(ArduinoController*)NULL) return -1;

  if (arduino->isOpened){
    arduino->isOpened = 0;
    return close(arduino->arduino_ID);
  }
  return -2;
}

int read_arduino(ArduinoController* arduino){
  if (arduino->arduino_ID==-1) return -1;
  tcflush(arduino->arduino_ID, TCIFLUSH);//Flush the input buffer
  unsigned char read_buf [IR_CODE_LENGTH+2];//IR code length + start & stop char
  memset(&read_buf, '\0', sizeof(read_buf));

  int num_bytes = read(arduino->arduino_ID, &read_buf, sizeof(read_buf));

  if (num_bytes < 0) {
    if (DEBUG_COMM) printf("%s: Error reading: %s",arduino->port ,strerror(errno));
    return 1;
  }

  if (DEBUG_COMM){
    printf("%s:Read %i bytes. Received message: ",arduino->port ,num_bytes);
    for (int i = 0; i < 6; ++i)printf("%02X ", read_buf[i]);
    printf("\n");
  }

  int ret_code=0;
  if (read_buf[0]!=START_CHAR || read_buf[IR_CODE_LENGTH+1]!=STOP_CHAR){
    if (DEBUG_COMM) printf("%s: Error reading: start & stop byte are not good\n",arduino->port);
    memset(arduino->ir_last_code, '\0', sizeof(read_buf));
    ret_code = 1;
  }else{
    memcpy(arduino->ir_last_code,read_buf+1,IR_CODE_LENGTH);
  }

  //Make sure we flush all the input buffer
  arduino->tty.c_cc[VTIME] = 1;
  while (num_bytes > 6){
    num_bytes = read(arduino->arduino_ID, &read_buf, sizeof(read_buf));
    tcflush(arduino->arduino_ID, TCIFLUSH);
  }
  arduino->tty.c_cc[VTIME] = 0;
  
  return ret_code;
}

//TODO: see if VMIN and VTIME are important
int write_arduino(ArduinoController* arduino, unsigned char* data){
  if (arduino->arduino_ID==-1) return -1;
  char write_buf[IR_CODE_LENGTH+2];
  write_buf[0] = STOP_CHAR;
  memcpy(&write_buf[1], data, IR_CODE_LENGTH);
  write_buf[IR_CODE_LENGTH+1] = START_CHAR;

  int errcode = write(arduino->arduino_ID, write_buf, IR_CODE_LENGTH+2);
  tcflush(arduino->arduino_ID, TCOFLUSH);

  if (errcode!=0) return -1;
  return 0;
}

int arduino_valid(ArduinoController* arduino){
  if (arduino==(ArduinoController*)NULL) return -1;
  if (arduino->arduino_ID==-1) return -1;
  if (!arduino->isOpened) return -1;

  return 0;
}