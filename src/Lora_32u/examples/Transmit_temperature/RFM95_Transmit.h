/*-------------------------------------------------------------
  RFM95 Transmit
  Lora feather32u4 
  Trung Nguyen 
  version 1.00 - 05/07/2021
--------------------------------------------------------------*/


#ifndef __RFM95_Transmit_h
#define __RFM95_Transmit_h

//#include <SPI.h>
//#include <RH_RF95.h>

// for feather32u4 
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
//RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Blinky on transmit
#define LED 13

void RFM_95_init();

void RFM_95_send();


// ------ Public variable -------------------------------------

#endif // __ESP32_ADC_H
