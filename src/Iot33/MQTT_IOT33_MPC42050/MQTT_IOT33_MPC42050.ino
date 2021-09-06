// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX

#include "Arduino.h"
#include "arduino_secrets.h"
#include "MCP_42050.h"


void setup()
{
 Serial.begin(9600);
 Mqtt_setup();
 MCP42050_init();
 
}

void loop()
{
//float x = read_raw_temperature(0);
  MCP42050_Resistor_out(read_raw_temperature(0));
  // MQTT send from pi one mess every 4 second
  delay(4000);
  
}
