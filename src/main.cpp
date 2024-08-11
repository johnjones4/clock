#include <Arduino.h>
#include <NMEAParser.h>

/*
 * There are three serial ports on the ESP known as U0UXD, U1UXD and U2UXD.
 * 
 * U0UXD is used to communicate with the ESP32 for programming and during reset/boot.
 * U1UXD is unused and can be used for your projects. Some boards use this port for SPI Flash access though
 * U2UXD is unused and can be used for your projects.
 * 
*/

#define RXD2 16
#define TXD2 17

#define ARG_TIME 0
#define ARG_LATITUDE 1
#define ARG_LATITUDE_DIRECTION 2
#define ARG_LONGITUDE 3
#define ARG_LONGITUDE_DIRECTION 4

NMEAParser<2> parser;

void errorHandler()
{
  Serial.print("*** Error : ");
  Serial.println(parser.error()); 
}

void messageHandler()
{
  Serial.print("Got message with ");
  Serial.print(parser.argCount());
  Serial.println(" arguments");

  char *arg = (char*)malloc(sizeof(char) * 16);
  if (parser.getArg(ARG_TIME, arg)) {
    Serial.println(arg);
  }
  free(arg);
}


void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(9600);
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));

  parser.setErrorHandler(errorHandler);
  parser.addHandler("GPGGA", messageHandler);
}

void loop() {
  while (Serial2.available()) {
    char c = char(Serial2.read());
    parser << c;
  }
}