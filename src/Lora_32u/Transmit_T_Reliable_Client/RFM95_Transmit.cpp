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

RHReliableDatagram manager(driver, CLIENT_ADDRESS);

// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB


void RFM_95_init()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  //Serial.println("LoRa reliable server");
  Serial.println("LoRa Reliable data client");
  
  // function to wait until receviced data from server
  //if (!manager.init()) {
  //  Serial.println("Lora radio init failed");
  //  while (1);
  //}
  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  driver.setFrequency(434E6);
  // read more on power management code.
  driver.setTxPower(23, false);
  driver.setSignalBandwidth(41700);
  driver.setCodingRate4(5);
  driver.setSpreadingFactor(7);

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

//uint8_t data[] = "Hello World!";
// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//uint8_t buf[manager.maxMessageLength()];
//uint8_t len = sizeof(buf);
//float prev_a;
//float a ;


void RFM_95_send(float myFloat)
//void RFM_95_send()
{
  Serial.println("Sending to rf95_reliable_datagram_server");
  //delay(1000);
  digitalWrite(LED, HIGH);
  //driver.sleep();
  //float R2_raw = myFloat;
  
  if (manager.sendtoWait((uint8_t *)(&myFloat), sizeof(myFloat), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    

    uint8_t len = sizeof(buf);
    uint8_t from;
    // if (manager.recvfromAckTimeout(buf, &len, 2000, &from)) , 2000 is time out
    if (manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      RH_RF95::printBuffer("Received: ", buf, len);
      //x = *((float *)buf);
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      //Serial.println(x);
      Serial.println((char*)buf);

    }
    else
    {
      Serial.println("No reply, is rf95_reliable_datagram_server running?");
    }
    //
  }
  else {
    //digitalWrite(LED, HIGH);
    //delay(100);
    Serial.println("sendtoWait failed");
    //digitalWrite(LED, LOW);
    //delay(1000);
  }

    digitalWrite(LED, LOW);
    

}

#endif // __ESP32_ADC_H
