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
//#include "MCP42050.h"
#include <RHReliableDatagram.h>

//#include "Arduino.h"

// private function
#define CLIENT_ADDRESS 1234
#define SERVER_ADDRESS 112233


// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 driver(RFM95_CS, RFM95_INT); // Rocket Scream Mini Ultra Pro with the RFM95W
// Blinky on receipt
#define LED 13

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, CLIENT_ADDRESS);


// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB

//

//const byte pot0            = 0x11;    // pot0 addr
//const byte pot1            = 0x12;    // pot1 addr

void RFM_95_init() {

  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.println("LoRa Reliable data client");

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
  //driver.setSignalBandwidth(31250);
  //driver.setCodingRate4(5);
  //driver.setSpreadingFactor(8);
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

uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//uint8_t buf[manager.maxMessageLength()];
//uint8_t len = sizeof(buf);
//
//float prev_a;
//float a ;

float RFM_95_received_with_key()
{

  Serial.println("Sending to rf95_reliable_datagram_server");
  float x;
  if (manager.sendtoWait(data, sizeof(data), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    digitalWrite(LED, HIGH);
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      RH_RF95::printBuffer("Received: ", buf, len);
      x = *((float *)buf);
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println(x);

    }
    else
    {
      Serial.println("No reply, is rf95_reliable_datagram_server running?");
    }
    //
    digitalWrite(LED, LOW);
  }
  else {
    digitalWrite(LED, HIGH);
    delay(100);
    Serial.println("sendtoWait failed");
    digitalWrite(LED, LOW);
  }

  delay(1000);

  return x;
}



#endif // __ESP32_ADC_H
