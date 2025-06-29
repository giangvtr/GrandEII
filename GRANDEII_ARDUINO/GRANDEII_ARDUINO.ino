#include <IRremote.h>

#define SERIAL_BAUD 115200
#define RECV_PIN 11

#define COMM_SERIAL Serial
#define COMM_BEGIN_CHAR 0x02
#define COMM_END_CHAR   0x03

#define DEBUG_ENABLE false
#define DEBUG_SERIAL if(DEBUG_ENABLE)Serial

IRrecv irrecv(RECV_PIN);
IRsend irsend;//IR_SEND_PIN = 9
decode_results results;

uint32_t currValue=0;
uint32_t lastValue=0;

void setup(){
  COMM_SERIAL.begin(SERIAL_BAUD);

  DEBUG_SERIAL.println("GRAND EII - Projet de C 3EII");
  irrecv.enableIRIn();
  irrecv.blink13(true);

  DEBUG_SERIAL.println("Fin du setup");
}

unsigned char recv_buffer[6];
unsigned long data;
void loop(){
  if (irrecv.decode(&results)){
      currValue = results.value;
      if((lastValue != currValue) && (currValue!=0xFFFFFFFF)){
        DEBUG_SERIAL.println("--Value changed--");
        lastValue = currValue;
      }
      DEBUG_SERIAL.print("Transmitting last value: ");
      DEBUG_SERIAL.print(lastValue, HEX);
      DEBUG_SERIAL.print(" & curr value: ");
      DEBUG_SERIAL.println(currValue, HEX);

      COMM_SERIAL.write(COMM_BEGIN_CHAR);
      COMM_SERIAL.write((char*) &lastValue, 4);
      COMM_SERIAL.write(COMM_END_CHAR);

      irrecv.resume();
  }
  if(Serial.available()>0){
    Serial.readBytes(recv_buffer, 6);
    DEBUG_SERIAL.print("Received data:");
    data=0;
    char start_char_verif = 0;
    char stop_char_verif = 0;

    stop_char_verif=  ((unsigned long) recv_buffer[0]);
    data|=            ((unsigned long) recv_buffer[4])<<24;
    data|=            ((unsigned long) recv_buffer[3])<<16;
    data|=            ((unsigned long) recv_buffer[2])<<8;
    data|=            ((unsigned long) recv_buffer[1])<<0;
    start_char_verif= ((unsigned long) recv_buffer[5]);
    
    DEBUG_SERIAL.println(data,HEX);

    if(start_char_verif!=COMM_BEGIN_CHAR || stop_char_verif!=COMM_END_CHAR){
      DEBUG_SERIAL.println("Data is not valid!");
    }else{
      DEBUG_SERIAL.println("Sending data to lamp");
      irsend.sendNEC(data, 32);
      DEBUG_SERIAL.println("Data sended");
    }

  }
}
