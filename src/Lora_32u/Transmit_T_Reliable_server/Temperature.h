/*-------------------------------------------------------------
  ADC Thermistor reading - header file
  Lora feather32u4 
  Trung Nguyen 
  version 1.00 - 05/07/2021
--------------------------------------------------------------*/


#ifndef __Temperature_h
#define __Temperature_h


#define THERMISTOR_PIN 1

//void ADC_init();

int tempSen01_read();

float tempSen_read_float();

// ------ Public variable -------------------------------------

#endif // __ESP32_ADC_H
