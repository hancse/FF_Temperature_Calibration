/*-------------------------------------------
  RFM95 Transmit
  Lora feather32u4
  Trung Nguyen 2021
  version 1.00 - 07/05/2021

  --------------------------------------------*/

#ifndef RFM95_Received_CPP
#define RFM95_Received_CPP
#include "RFM95_Received.h"
#include <SPI.h>
#include <RH_RF95.h>
#include "MCP42050.h"
#include <Speck.h>
#include <RHEncryptedDriver.h>

//#include "Arduino.h"

// private function

// Change to 434.0 or other frequency, must match RX's freq!
//#define RF95_FREQ 915.0
#define RF95_FREQ 434.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blinky on receipt
#define LED 13

Speck myCipher;   // Instanciate a Speck block ciphering
RHEncryptedDriver myDriver(rf95, myCipher); // Instantiate the driver with those two

//float frequency = 868.0; // Change the frequency here.
unsigned char encryptkey[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}; // The very secret key

//

const byte pot0            = 0x11;    // pot0 addr
const byte pot1            = 0x12;    // pot1 addr

void RFM_95_init()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  //Serial.begin(9600);
  //while (!Serial) {
  //  delay(1);
  //}
  //delay(100);

  Serial.println("LoRa Simple_Encrypted Server");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);
  //rf95.setTxPower(13);
  rf95.setSignalBandwidth(41700);
  //driver.setSignalBandwidth(31250);
  rf95.setCodingRate4(5);
  rf95.setSpreadingFactor(10);
  myCipher.setKey(encryptkey, sizeof(encryptkey));
  delay(4000);
  Serial.println("Setup completed");
}


void RFM_95_received() {
  /*
       if (rf95.available())
    {
    // Should be a message for us now

    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);
      Serial.print("Got resistor value from remote device: ");
      //Serial.println((char*)buf);
      // Convert received message to float.
      float x = *((float *)buf);

      Serial.println(x);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      //resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
      int pos = (x -161)*256/50000;
      //pos = pos + 2
      setPotWiper(pot0, pos +1);
      setPotWiper(pot1, pos +2);
      Serial.println(pos);

      float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
      //int Vo = analogRead(THERMISTOR_PIN);
      float R1 = 9820;
      //float R2 = R1 * (1023.0 / (float)Vo - 1.0);
      float logR2 = log(x);
      float T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
      float Tc = T - 273.15;
      //float Tf = (Tc * 9.0)/ 5.0 + 32.0;

      Serial.print("Temperature: ");
      //Serial.print(Tf);
      //Serial.print(" F; ");
      Serial.println(Tc);
      //Serial.println(R2);
      // Send a reply
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(LED, LOW);
    }
    else
    {
      Serial.println("Receive failed");
    }
    }


  */

}
float prev_a;
float a ;

void RFM_95_received_with_key()
{
  if (myDriver.available()) {
    // Should be a message for us now

    uint8_t buf[myDriver.maxMessageLength()];
    uint8_t len = sizeof(buf);

    if (myDriver.recv(buf, &len)) {
      digitalWrite(LED, HIGH);
      RH_RF95::printBuffer("Received: ", buf, len);

      //Serial.println((char*)buf);
      // Convert received message to float.


      float x = *((float *)buf);

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
        Serial.println("Reading error set x to default value 10000 ohm at 25 degree");
      }


      Serial.println(x);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      //resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
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
      //Serial.print(Tf);
      //Serial.print(" F; ");
      Serial.println(Tc);
      //Serial.println(R2);
      // Send a reply
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(LED, LOW);
    }
    else {

      Serial.println("Receive failed");
    }

  }

}

#endif // __ESP32_ADC_H
