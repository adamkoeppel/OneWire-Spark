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


//set up ADC voltage channels
#define V_AD 1
#define V_DD 0


void setup(void) {
  Serial.begin(9600);  // local hardware test only, for Spark set to 57600, for conventional set to 9600

//put valve configuration here


}

void loop(void) {
  byte i;
  byte type_s = 2;
  byte data_T[12];
  byte data_V[12];
  byte addr[8];
  float celsius, fahrenheit, voltage, voltage2, temperature2;

/*
  oneWire.reset();
  oneWire.select(DS2438_address2);      // Just do one at a time for testing

  oneWire.write(0x44);        // start conversion, with parasite power on at the end
  
  delay(2900);     // maybe 750ms is enough, maybe not, I'm shooting for 1 reading per second
                    // prob should set to min reliable and check the timer for 1 second intervals
                    // but that's a little fancy for test code
                    
  // we might do a oneWire.depower() here, but the reset will take care of it.


  
  oneWire.reset();
  oneWire.select(DS2438_address2);    
  oneWire.write(0xB8,0);         // Recall Memory 0
  oneWire.write(0x00,0);         // Recall Memory 0

  oneWire.reset();
  oneWire.select(DS2438_address2);    
  oneWire.write(0xBE, 0);         // Read Scratchpad 0
  oneWire.write(0x00,0);         // Recall Memory 0

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data_T[i] = oneWire.read();
  }
  
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
  Serial.println();
  Serial.print("  Data T = ");
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
  Serial.println();
*/

  temperature2 = MeasTemperature_2438(DS2438_address2);

  Serial.println();
  Serial.print("  Data T2 = ");
  Serial.print("  Temperature2 = ");
  Serial.print(temperature2);
  Serial.println();

/*
  oneWire.reset();
  oneWire.select(DS2438_address2);      // Just do one at a time for testing

  oneWire.write(0xb4);        // start conversion, with parasite power on at the end
  
  delay(2900);     // maybe 750ms is enough, maybe not, I'm shooting for 1 reading per second
                    // prob should set to min reliable and check the timer for 1 second intervals
                    // but that's a little fancy for test code
                    
  // we might do a oneWire.depower() here, but the reset will take care of it.


  
  oneWire.reset();
  oneWire.select(DS2438_address2);    
  oneWire.write(0xB8,0);         // Recall Memory 0
  oneWire.write(0x00,0);         // Recall Memory 0

  oneWire.reset();
  oneWire.select(DS2438_address2);    
  oneWire.write(0xBE, 0);         // Read Scratchpad 0
  oneWire.write(0x00,0);         // Recall Memory 0

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data_V[i] = oneWire.read();
  }
  
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw2 = (data_V[1] << 8) | data_V[0];
  if (type_s) {
    if (type_s==1) {    // DS18S20
      raw2 = raw2 << 3; // 9 bit resolution default
      if (data_V[7] == 0x10) {
        // "count remain" gives full 12 bit resolution
        raw2 = (raw2 & 0xFFF0) + 12 - data_V[6];
      }
      celsius = (float)raw2 / 16.0;
    }else{ // type_s==2 for DS2438
      //if (data_V[2] > 127) data_V[2]=0;
      //data_V[1] = data_V[1] >> 3;
      voltage = (((data_V[4] << 8) & 0x00300) | (data_V[3] & 0x0ff));
      voltage = voltage / 100;
    }
  } 
//  else {  // DS18B20 and DS1822
//    byte cfg = (data_V[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
//    if (cfg == 0x00) raw2 = raw2 & ~7;  // 9 bit resolution, 93.75 ms
//    else if (cfg == 0x20) raw2 = raw2 & ~3; // 10 bit res, 187.5 ms
//    else if (cfg == 0x40) raw2 = raw2 & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
//    celsius = (float)raw2 / 16.0;
//  }


  
  // debug output
  Serial.println();
  Serial.print("  Data V = ");
  Serial.print("  Voltage = ");
  Serial.print(voltage);
  Serial.println();
*/

  voltage2 = MeasADC(DS2438_address2, V_AD);

  Serial.println();
  Serial.print("  Data V2 = ");
  Serial.print("  Voltage2 = ");
  Serial.print(voltage2);
  Serial.println();

}


float MeasADC(uint8_t address[8], int source)
{
  int n, a[9]; 
  float v;
  
  oneWire.reset();
  oneWire.select(address);  
  oneWire.write( 0x4e, 0);   // write scratchpad
  oneWire.write( 0x00, 0);   // setup for Vdd or A/D
  if (source == V_AD)
  {
      oneWire.write( 0x00, 0);  // Vad
  }
  else
  {
      oneWire.write( 0x08, 0);  // Vdd
  }     

  delay(1000);     // maybe 750ms
  
  oneWire.reset();
  oneWire.select(address);  
  oneWire.write( 0xb4, 0);  // perform A/D
  delay(1000);  // wait for A/D to complete

  oneWire.reset();  //reset
  oneWire.select(address);  
  oneWire.write( 0xb8, 0);   //recall memory
  oneWire.write( 0x00, 0);  // recall to scratchpad  

  oneWire.reset();
  oneWire.select(address);  
  oneWire.write( 0xbe, 0);  //reac scratchpad
  oneWire.write( 0x00, 0);  // read scratchpad
  
  for (n=0; n<9; n++)
  {
      a[n] = oneWire.read(); 
       //Serial.print(a[n]);
       //Serial.print("...?n");
       //delay(100);
  }  
       
  v = a[4] * 256 + a[3];    
  v = v / 100;
  return(v); 
}

float MeasTemperature_2438(uint8_t address[8]) 
{
     int data_T[12], i;
     int type_s = 2;
     float t;

  oneWire.reset();
  oneWire.select(address);      // Just do one at a time for testing
  oneWire.write(0x44);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms
      
  oneWire.reset();
  oneWire.select(address);    
  oneWire.write(0xB8,0);         // Recall Memory 0
  oneWire.write(0x00,0);         // Recall Memory 0

  oneWire.reset();
  oneWire.select(address);    
  oneWire.write(0xBE, 0);         // Read Scratchpad 0
  oneWire.write(0x00,0);         // Recall Memory 0

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data_T[i] = oneWire.read();
  }
  
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
      t = (float)raw / 16.0;
    }else{ // type_s==2 for DS2438
      if (data_T[2] > 127) data_T[2]=0;
      data_T[1] = data_T[1] >> 3;
      t = (float)data_T[2] + ((float)data_T[1] * .03125);
    }
  } else {  // DS18B20 and DS1822
    byte cfg = (data_T[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
    t = (float)raw / 16.0;
  }
  return(t);
}

