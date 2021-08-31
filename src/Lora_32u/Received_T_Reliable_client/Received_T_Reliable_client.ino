// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX

#include "Arduino.h"
#include "RFM95_Received.h"
#include "MCP42050.h"


void setup()
{
 //Serial.begin(9600);
 RFM_95_init();
 MCP42050_init();
 
}

void loop()
{
  // only received message when transmit
  //RFM_95_received();
  //RFM_95_received_with_key();
  MCP42050_Resistor_out();
  
}
