// This #include statement was automatically added by the Spark IDE.
#include "OneWire.h"

// OneWire DS18S20, DS18B20, DS1822, DS2438 Temperature Example
//
// https://github.com/Hotaman/OneWireSpark
//
// Thanks to all who have worked on this demo!
// I just made some minor tweeks for the spark core
// and added support for the DS2438 battery monitor
// 6/2014 - Hotaman

// Define the pins we will use
int ow = 3;    // put the onewire bus on D3 (or 3 for a regular Arduino)

//create the oneWire instance
OneWire  oneWire(ow);  // OneWire commands will be prefixed with oneWire

// define the 1-Wire addresses of the DS2438 battery monitors here (lsb first)
uint8_t DS2438_address1[] = { 0x26, 0x5B, 0xDD, 0xD6, 0x01, 0x00, 0x00, 0xA6 };       //device #1
uint8_t DS2438_address2[] = { 0x26, 0x69, 0xDD, 0xD6, 0x01, 0x00, 0x00, 0x25 };       //device #2
uint8_t DS2438_address3[] = { 0x26, 0x6D, 0xDD, 0xD6, 0x01, 0x00, 0x00, 0xF9 };       //device #3


void setup(void) {
  Serial.begin(9600);  // local hardware test only, for Spark set to 57600, for conventional set to 9600

//put valve configuration here


}

void loop(void) {
  byte i;
//  byte present = 0;
  byte type_s = 2;
  byte data_T[12];
  byte data_V[12];
  byte addr[8];
  float celsius, fahrenheit;

  oneWire.reset();
  oneWire.select(DS2438_address2);      // Just do one at a time for testing

  oneWire.write(0x44);        // start conversion, with parasite power on at the end
  
  delay(2900);     // maybe 750ms is enough, maybe not, I'm shooting for 1 reading per second
                    // prob should set to min reliable and check the timer for 1 second intervals
                    // but that's a little fancy for test code
                    
  // we might do a oneWire.depower() here, but the reset will take care of it.
  
//  present = 
  oneWire.reset();
  oneWire.select(DS2438_address2);    
  oneWire.write(0xB8,0);         // Recall Memory 0
  oneWire.write(0x00,0);         // Recall Memory 0

//  present = 
  oneWire.reset();
  oneWire.select(DS2438_address2);    
  oneWire.write(0xBE, 0);         // Read Scratchpad 0
  oneWire.write(0x00,0);         // Recall Memory 0


  Serial.print("  Data T = ");
//  Serial.print(present, HEX);
//  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data_T[i] = oneWire.read();
//    Serial.print(data_T[i], HEX);
//    Serial.print(" ");
  }
//  Serial.print(" CRC=");
//  Serial.print(OneWire::crc8(data_T, 8), HEX);
  Serial.println();


  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data_T[1] << 8) | data_T[0];
  if (type_s) {
    if (type_s==1) {    // DS18S20
      raw = raw << 3; // 9 bit resolution default
      if (data_T[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw = (raw & 0xFFF0) + 12 - data_T[6];
      }
      celsius = (float)raw / 16.0;
    }else{ // type_s==2 for DS2438
      if (data_T[2] > 127) data_T[2]=0;
      data_T[1] = data_T[1] >> 3;
      celsius = (float)data_T[2] + ((float)data_T[1] * .03125);
    }
  } else {  // DS18B20 and DS1822
    byte cfg = (data_T[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
    celsius = (float)raw / 16.0;
  }
  fahrenheit = celsius * 1.8 + 32.0;
  // end of test code
  
  // debug output
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
  Serial.println();
}
