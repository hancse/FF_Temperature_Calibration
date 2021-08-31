/*-------------------------------------------
  ADC Converter - function file
  Lora feather32u4 
  Trung Nguyen 2021
  version 1.00 - 07/05/2021
  
--------------------------------------------*/ 




#ifndef Temperature_CPP
#define Temperature_CPP
#include "Temperature.h"
#include "Arduino.h"


// Steinhart-Hart approximation (NTC)
//float logR2, R2, T, Tc, Tf;
//float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int tempSen01_read() {
  
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
  int Vo = analogRead(THERMISTOR_PIN);
  float R1 = 9820;
  float R2 = R1 * (1023.0 / (float)Vo - 1.0);
  float logR2 = log(R2);
  float T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  int Tc = int(T - 273.15);
  float Tf = (Tc * 9.0)/ 5.0 + 32.0; 

  //Serial.print("Temperature: "); 
  //Serial.print(Tf);
  //Serial.print(" F; ");
  //Serial.print(Tc);
  //Serial.println(R2);   

  delay(500);
  return Tc;
}

float tempSen_read_float() {
  
  float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
  int Vo = analogRead(THERMISTOR_PIN);
  //float R1 = 10000;
  float R1 = 9820;
  float R2 = R1 * (1023.0 / (float)Vo - 1.0);
  float logR2 = log(R2);
  float T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  float Tc = T - 273.15;
  float Tf = (Tc * 9.0)/ 5.0 + 32.0; 

  Serial.print("Temperature: "); 
  //Serial.print(Tf);
  //Serial.print(" F; ");
  Serial.print(Tc);
  Serial.println(" C");   

  delay(500);
  return R2;
}

#endif //__ESP32_ADC_CPP
