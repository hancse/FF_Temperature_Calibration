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
#include "RFM95_Received.h"

// private function

const byte csPin           = 5                                                                                                                                                                                                                                                                                                                                                                      ;       // MCP42100 chip select pin
const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
const long rAB             = 50000;  // 100k pot resistance between terminals A and B
const byte rWiper          = 161;     // 125 ohms pot wiper resistance
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

float prev_a;
float a ;

void MCP42050_Resistor_out() {

  float x = RFM_95_received_with_key();
  Serial.println(x);
  
  if (x >= 3500 && x <= 29000) {
        a = x;
        prev_a = a;
        Serial.print("value of a:");
        Serial.println(a);
        Serial.print("Got resistor value from remote device: ");
      }
      else {
        a = prev_a;
        //x = 10000;
        x = a;
        Serial.print("value of a:");
        Serial.println(a);
        Serial.println("Reading error set x to previous or default value 10000 ohm at 25 degree");
      }


      Serial.println(x);
      Serial.print("RSSI: ");

      //resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
      // rwiper = 161
      int pos = (x - 161) * 256 / 50000;
      //pos = pos + 2
      setPotWiper(pot0, pos + 1);
      setPotWiper(pot1, pos + 2);
      Serial.println(pos);

      float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
      //int Vo = analogRead(THERMISTOR_PIN);
      float R1 = 9820;
      //float R2 = R1 * (1023.0 / (float)Vo - 1.0);
      float logR2 = log(x);
      float T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));
      float Tc = T - 273.15;
      //float Tf = (Tc * 9.0)/ 5.0 + 32.0;

      Serial.print("Temperature: ");
      Serial.println(Tc);
      //Serial.println(R2);

}

void setPotWiper(int addr, int pos) {

  pos = constrain(pos, 0, 171);            // limit wiper setting to range of 0 to 255

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
  Serial.print(resistanceWB);
  Serial.println(" ohms");

}

#endif // 
