/*-------------------------------------------
  RFM95 Transmit
  Lora feather32u4
  Trung Nguyen 2021
  version 1.00 - 07/05/2021

  --------------------------------------------*/

#ifndef RFM95_Transmit_CPP
#define RFM95_Transmit_CPP
#include "RFM95_Transmit.h"
#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include "Temperature.h"

//#include "Arduino.h"

// private function
#define CLIENT_ADDRESS 1234
#define SERVER_ADDRESS 112233

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 driver(RFM95_CS, RFM95_INT);
//RH_RF95 driver(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB

void RFM_95_init()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.println("LoRa reliable server");

  // manual reset
  //digitalWrite(RFM95_RST, LOW);
  //delay(10);
  //digitalWrite(RFM95_RST, HIGH);
  //delay(10);
  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  //driver.setFrequency(434E6);
  driver.setTxPower(23, false);
  //driver.setSignalBandwidth(41700);
  //driver.setCodingRate4(5);
  //driver.setSpreadingFactor(8);
  
  //driver.setTxPower(23, false);
  // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
  // transmitter RFO pins and not the PA_BOOST pins
  // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true.
  // Failure to do that will result in extremely low transmit powers.
  //  driver.setTxPower(14, true);
  // You can optionally require this module to wait until Channel Activity
  // Detection shows no activity on the channel before transmitting by setting
  // the CAD timeout to non-zero:
  //  driver.setCADTimeout(10000);

  Serial.println("Setup completed");
}

//uint8_t data[] = "And hello back to you";
// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

void RFM_95_send(float myFloat)
//void RFM_95_send()
{
  //Read raw Temperature value in ohm
  //float myFloat  = tempSen_read_float();
  
  
  if (manager.available())
  {
    digitalWrite(LED, HIGH);
    //delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;

    if (manager.recvfromAck(buf, &len, &from))
    {
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);

      // Send a reply back to the originator client
      //myDriver.send((uint8_t *)(&myFloat), sizeof(myFloat)); // Send out ID + Sensor data to LoRa gateway
      if (!manager.sendtoWait((uint8_t *)(&myFloat), sizeof(myFloat), from))
        Serial.println("sendtoWait failed");
    }
    digitalWrite(LED, LOW);
  }
    
}

#endif // __ESP32_ADC_H
