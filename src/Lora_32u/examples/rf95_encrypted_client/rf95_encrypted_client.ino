// LoRa Simple Hello World Client with encrypted communications 
// In order for this to compile you MUST uncomment the #define RH_ENABLE_ENCRYPTION_MODULE line
// at the bottom of RadioHead.h, AND you MUST have installed the Crypto directory from arduinolibs:
// http://rweather.github.io/arduinolibs/index.html
//  Philippe.Rochat'at'gmail.com
//  06.07.2017

#include <RH_RF95.h>
#include <RHEncryptedDriver.h>
#include <Speck.h>
#include <SPI.h>

// for feather32u4 
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7


// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

//RH_RF95 rf95;     // Instanciate a LoRa driver
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

Speck myCipher;   // Instanciate a Speck block ciphering
// uncomment radio head.h file
RHEncryptedDriver myDriver(rf95, myCipher); // Instantiate the driver with those two

//float frequency = 868.0; // Change the frequency here. 
unsigned char encryptkey[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The very secret key
char HWMessage[] = "Hello World ! I'm happy if you can read me";
uint8_t HWMessageLen;

void setup()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  HWMessageLen = strlen(HWMessage);
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  Serial.println("LoRa Simple_Encrypted Client");
  delay(100);
  
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
  //rf95.setTxPower(13);
  myCipher.setKey(encryptkey, sizeof(encryptkey));
  Serial.println("Waiting for radio to setup");
  delay(1000);
  Serial.println("Setup completed");
}

void loop()
{
  uint8_t data[HWMessageLen+1] = {0};
  for(uint8_t i = 0; i<= HWMessageLen; i++) data[i] = (uint8_t)HWMessage[i];
  myDriver.send(data, sizeof(data)); // Send out ID + Sensor data to LoRa gateway
  Serial.print("Sent: ");
  Serial.println((char *)&data);
  delay(4000);
}
