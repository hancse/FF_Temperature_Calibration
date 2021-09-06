/*-------------------------------------------------------------
  RFM95 MCP42050 Read
  Lora feather32u4 
  Trung Nguyen 
  version 1.00 - 05/07/2021
--------------------------------------------------------------*/


#ifndef __MCP42050_h
#define __MCP42050_h
//#include <SPI.h>


void MCP42050_init();

void MCP42050_Resistor_out(float x);

void setPotWiper(int addr, int pos);


#endif // 
