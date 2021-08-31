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
#include <RHEncryptedDriver.h>
#include <Speck.h>
#include "Temperature.h"

//#include "Arduino.h"

// private function
// for feather32u4 
//#define RFM95_CS 8
//#define RFM95_RST 4
//#define RFM95_INT 7

// Change to 434.0 or other frequency, must match RX's freq!
//#define RF95_FREQ 915.0
#define RF95_FREQ 434.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);


Speck myCipher;   // Instanciate a Speck block ciphering
RHEncryptedDriver myDriver(rf95, myCipher); // Instantiate the driver with those two

unsigned char encryptkey[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The very secret key

//char HWMessage[] = "FZ*#12";
//uint8_t HWMessageLen;

void RFM_95_init() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  //HWMessageLen = strlen(HWMessage);
  //Serial.begin(9600);
  //while (!Serial) {
  //  delay(1);
  //}

  //delay(100);

  Serial.println("LoRa Simple_Encrypted Client");

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
  rf95.setSignalBandwidth(41700);
  rf95.setCodingRate4(5);
  rf95.setSpreadingFactor(10);
  //rf95.setTxPower(13);
  //key
  myCipher.setKey(encryptkey, sizeof(encryptkey));
  Serial.println("Waiting for radio to setup");
  delay(1000);
  Serial.println("Setup completed");
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void RFM_95_send()
{
  digitalWrite(LED, HIGH);
  delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
    //tempSen01_read();
   if (tempSen_read_float() > 3500 && tempSen_read_float()< 30000)
      {
       Serial.println("Read Raw Temperature Success...");
        }         
  Serial.println("Transmitting..."); // Send a message to rf95_server
  
  //int temp = tempSen01_read();
  float myFloat  = tempSen_read_float();
  Serial.print("Sending "); Serial.println(myFloat);
  Serial.println("Sending...");
  delay(10);
 
  myDriver.send((uint8_t *)(&myFloat), sizeof(myFloat)); // Send out ID + Sensor data to LoRa gateway
  //rf95.send((uint8_t *)(&myFloat), sizeof(myFloat));
  //rf95.send((uint8_t *)temp, 20);

  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  //rf95.waitPacketSent();
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  //Turn led off
  digitalWrite(LED, LOW);
  
  Serial.println("Waiting for reply...");
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is there a listener around?");
  }

}

#endif // __ESP32_ADC_H
