/******************************************************************************
  Example1-SerialRead
  QwiicRF Library Serial Read Example

  Nick Poole @ SparkFun Electronics
  June 21, 2018
  https://github.com/sparkfun/SparkFun_QwiicRF_Library

  This example checks once per second to see if the attached QwiicRF has received
  any payload. If it has, the payload is written to the Serial terminal.

  Hardware Resources:
  - SparkFun BlackBoard (https://www.sparkfun.com/products/14669)
  - QwiicRF Board (https://www.sparkfun.com/products/14788)
  - Qwiic Cable (https://www.sparkfun.com/products/14426)

  Development environment specifics:
  Arduino 1.8.5
  QwiicRF Board
  SparkFun BlackBoard (any 'duino should do)
******************************************************************************/

#include <QwiicRF.h>
#include "U8glib.h"

String incoming = "";
String PM1_0,PM2_5,PM10;
String data_buffer[]={"pm1.0:","","pm2.5:","","pm10:",""};
uint8_t save_flag =0,k=0;
QwiicRF Radio;
U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);             // I2C
void setup() {

  Serial.begin(9600);
  Radio.begin(0x35); //Default I2C Address for QwiicRF
  u8g.firstPage(); 
  do {     
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 20);
    u8g.print("PMSA003I:");
    u8g.setPrintPos(0, 40);
    u8g.print("pm1.0:");
    u8g.setPrintPos(0, 60);
    u8g.print("pm2.5:");
    u8g.setPrintPos(0, 80);
    u8g.print("pm10:");
    } while( u8g.nextPage() ); 
}

void loop() {

    while ( Radio.hasPayload() == false ) {
    delay(1000);
  }

  //Serial.println(Radio.read());
  
  incoming=Radio.read();
  if(incoming=="#")
  {
    save_flag=0;
    k=0;
    for(uint8_t j=0;j<6;j++)
      {Serial.println(data_buffer[j]);} 
    u8g.firstPage();  
    do {     
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 20);
    u8g.print("PMSA003I:");
    u8g.setPrintPos(0, 40);
    u8g.print(data_buffer[0]);u8g.setPrintPos(50, 40);u8g.print(data_buffer[1]);u8g.print("ug/m3");
    u8g.setPrintPos(0, 60);
    u8g.print(data_buffer[2]);u8g.setPrintPos(50, 60);u8g.print(data_buffer[3]);u8g.print("ug/m3");
    u8g.setPrintPos(0, 80);
    u8g.print(data_buffer[4]);u8g.setPrintPos(50, 80);u8g.print(data_buffer[5]);u8g.print("ug/m3");
    } while( u8g.nextPage() );     
    }
  if(save_flag==1)
  {
    data_buffer[k++]=incoming;
    }
  if(incoming=="@")
  {
    save_flag=1;
    }
 
}
