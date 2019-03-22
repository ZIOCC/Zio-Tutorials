/* Andrea Toscano
Universita' degli Studi di Milano

Sample script - Reading some info from U-Blox Neo M8N GPS/GLONASS
*/

#include <Wire.h>
#include <U8g2lib.h>
#include "Ublox.h"
#include <SoftwareSerial.h>

#define SERIAL_BAUD 115200
#define GPS_BAUD 9600
#define N_FLOATS 4


Ublox M8_Gps;
U8G2_SSD1327_EA_W128128_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 5, /* data=*/ 4, /* reset=*/ U8X8_PIN_NONE);
SoftwareSerial ss(2, 4);

// Altitude - Latitude - Longitude - N Satellites
float gpsArray[N_FLOATS] = {0, 0, 0, 0};

void setup() {
  
   u8g2.begin();
   Serial.begin(SERIAL_BAUD);
   ss.begin(GPS_BAUD);
 
}

void loop() {
   if(!ss.available())
		return;

  while(ss.available()){
        char c = ss.read();
         if (M8_Gps.encode(c)) {
          gpsArray[0] = M8_Gps.altitude;
          gpsArray[1] = M8_Gps.latitude;
          gpsArray[2] = M8_Gps.longitude; 
          gpsArray[3] = M8_Gps.sats_in_use;
        }
  }
  for(byte i = 0; i < N_FLOATS; i++) {
    Serial.print(gpsArray[i], 6);Serial.print(" ");
    u8g2.clearBuffer();	
    u8g2.setCursor(0, 0);
    u8g2.setFont(u8g2_font_logisoso32_tf);
    u8g2.print(gpsArray[i], 6);Serial.print(" ");
    u8g2.sendBuffer();	
  }
  Serial.println("");
  u8g2.println("");
}
