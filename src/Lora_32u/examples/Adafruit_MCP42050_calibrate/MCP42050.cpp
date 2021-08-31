/*-------------------------------------------
  RFM95  MCP42050 Read
  Lora feather32u4 
  Trung Nguyen 2021
  version 1.00 - 07/05/2021
  
--------------------------------------------*/ 

#ifndef MCP42050_CPP
#define MCP42050_CPP
#include "MCP42050.h"
#include <SPI.h>

// private function

const byte csPin           = 5                                                                                                                                                                                                                                                                                                                                                                      ;       // MCP42100 chip select pin
const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
//const long rAB             = 50000;  // 50k pot resistance between terminals A and B
const long rAB             = 100000;  // 100k pot resistance between terminals A and B

//const byte rWiper          = 161;     // 125 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr
const byte pot1            = 0x12;    // pot1 addr
const byte potBoth         = 0x13;    // pot0 and pot1 simultaneous addr
const byte pot0Shutdown    = 0x21;    // pot0 shutdown
const byte pot1Shutdown    = 0x22;    // pot1 shutdown
const byte potBothShutdown = 0x23;    // pot0 and pot1 simultaneous shutdown

void MCP42050_init() {

  Serial.begin(9600);

  digitalWrite(csPin, HIGH);        // chip select default to de-selected
  pinMode(csPin, OUTPUT);           // configure chip select as output
  SPI.begin();

  Serial.println("MCP42100 Demo");
  Serial.println("First pot1 wiper moves from min to max in approx. 25% increments.");
  Serial.println("Then pot0 wiper moves from min to max and max to min in single increments");
  Serial.println();

}


void MCP42050_Resistor_out(float rWiper) {

  // move pot1 wiper manually to specific positions
  Serial.println("Pot 1 Control ");
  Serial.println(rWiper);
  delay(1000);
  setPotWiper(pot1, 255, rWiper);                    // minimum resistance
  setPotWiper(pot0,255, rWiper);                  // position 10 over 256 of 50000 (50 kohm resistor)

}

void setPotWiper(int addr, int pos, float rWiper) {

  pos = constrain(pos, 0, 80);            // limit wiper setting to range of 0 to 255
  digitalWrite(csPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper position
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // de-select chip

  // print pot resistance between wiper and B terminal
  //long Rwa= (100*10^3)*(256-255)/256 + 125;
  long resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
  Serial.print("Wiper position: ");
  Serial.print(pos);
  Serial.print(" Resistance wiper to B terminal: ");
  Serial.println(resistanceWB);
  Serial.println(" ohms");

}

float Thermistor_read()
{
  int ThermistorPin = 1;
  int Vo;

  //Steinhart-Hart approximation (NTC)
  float R1 = 10000;
  float R2;
  //float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  //logR2 = log(R2);
  //T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  //Tc = T - 273.15;
  //Tf = (Tc * 9.0)/ 5.0 + 32.0; 

  Serial.println(R2);
  //Serial.print(" F; ");
  //Serial.print(Tc);
  //Serial.println(" C");   

  delay(500);
  return R2;
  
  }
#endif // 
