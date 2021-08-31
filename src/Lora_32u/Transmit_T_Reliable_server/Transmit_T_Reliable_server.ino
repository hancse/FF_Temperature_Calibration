/*------------------------------------------------------------*-
  MAIN - functions file
  Lora feather32u4 
  Trung Nguyen - 2021
  version 1.00 - 05/07/2021
--------------------------------------------------------------*/


//#ifdef SYSTEM_DEBUG
//#else
#include "Temperature.h"
#include "RFM95_Transmit.h"
#include "Arduino.h"
//#endif


///////////////////////////////////////MAIN FUNCTION/////////////////////////////////////
// ---------------------------------- SETUP PROTOCOL -------------------------------------------
void setup() {
  Serial.begin(9600);
  RFM_95_init();
}// end setup

// ---------------------------------- LOOP -------------------------------------------
void loop() {

    RFM_95_send(tempSen_read_float());
    //RFM_95_send();
  
}//end loop
