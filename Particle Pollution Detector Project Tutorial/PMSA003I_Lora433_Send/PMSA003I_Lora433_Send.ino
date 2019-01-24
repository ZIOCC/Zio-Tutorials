// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Wire.h>
#include <QwiicRF.h>
QwiicRF Radio;

#define address 0x12
#define HEADER_H  0x42
#define HEADER_L  0x4D
#define FRAME_LENGTH  0x1C

uint8_t COMMAND = 0x00;
uint8_t Sensor_Databufer[32];

unsigned char cur_rx_data;
unsigned char pre_rx_data;
unsigned char main_status=0;
unsigned char recv_buff[256]={0};
unsigned char recv_buff_index=0;
unsigned char incomingByte;

int pm1_0_cf_1=0;
int pm2_5_cf_1=0;
int pm10_cf_1=0;

int pm1_0=0;
int pm2_5=0;
int pm10=0;

unsigned short check_sum;
char message[32];
char divi = ',';

void constring(char *s, char *t, char *q)
{
  uint8_t i,j;
  for(i-0;s[i]!='\n';i++)
  {
    q[i]=s[i];
    }
   for(j=0;t[j]!='\n';j++)
   {
    q[i+j]=t[j];
    }
   q[i+j]='\n';
  }
void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  Radio.begin(0x35);
  Radio.setReliableSendTimeout(5); // Change the reliable send timeout to 5 seconds
  pinMode(13,OUTPUT);
}

void loop() {
  digitalWrite(13,HIGH);
  Wire.requestFrom(address, 32);
  while(Wire.available())
  {
    cur_rx_data = Wire.read();
    switch(main_status)
     {
      case 0:
         if( cur_rx_data == HEADER_L )
         {
            if( pre_rx_data ==  HEADER_H )
            {
                 main_status++;
                 
                 check_sum += pre_rx_data;
                 check_sum += cur_rx_data;
                 
                 cur_rx_data = 0;
                 pre_rx_data = 0;

                 
            }
         }else
         {
            pre_rx_data = cur_rx_data;
         }
        break;
        case 1:
        if( cur_rx_data == FRAME_LENGTH )
         {
            if( pre_rx_data ==  0x00 )
            {
                 main_status++;
                 
                 check_sum += pre_rx_data;
                 check_sum += cur_rx_data;
                 
                 cur_rx_data = 0;
                 pre_rx_data = 0;
            }
         }else
         {
            pre_rx_data = cur_rx_data;
         }
        break;
        case 2:
        recv_buff[recv_buff_index++] = cur_rx_data;
        check_sum += cur_rx_data;
        if( recv_buff_index == 26)
        {
             main_status++;
          
             cur_rx_data = 0;
             pre_rx_data = 0;
        }
        break;
        case 3:
        recv_buff[recv_buff_index++] = cur_rx_data;
        if( recv_buff_index == 28)
        { 
            if( check_sum == ( (recv_buff[26]<<8) + recv_buff[27]) )
            {
                recv_buff_index = 0;  
                pm1_0_cf_1 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm2_5_cf_1 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm10_cf_1 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                
                pm1_0 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm2_5 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                recv_buff_index += 2;
                pm10 = (recv_buff[recv_buff_index] << 8) + recv_buff[recv_buff_index+1];
                
                Serial.print("PMSA003I:    ");
//                Serial.print("pm1.0_cf_1:");Serial.print(pm1_0_cf_1,DEC);Serial.print("ug/m3");Serial.print("  ");
//                Serial.print("pm2.5_cf_1:"); Serial.print(pm2_5_cf_1,DEC);Serial.print("ug/m3");Serial.print("  ");
//                Serial.print("pm10_cf_1:");Serial.print(pm10_cf_1,DEC);Serial.print("ug/m3");Serial.print("  ");
                Serial.print("pm1.0:  ");Serial.print(pm1_0,DEC);Serial.print("ug/m3");Serial.print("  ");
                Serial.print("pm2.5:  ");Serial.print(pm2_5,DEC);Serial.print("ug/m3");Serial.print("  ");
                Serial.print("pm10:  ");Serial.print(pm10,DEC);Serial.println("ug/m3");
                Radio.sendReliable(0xFF, "@");
                while (Radio.waitingForAck()) {
                  delay(1000);
                } 
                delay(2000); 
                Radio.sendReliable(0xFF, "pm1.0:");
                while (Radio.waitingForAck()) {
                  delay(1000);
                } 
                delay(2000);               
                Radio.sendReliable(0xFF, (String)pm1_0);
                while (Radio.waitingForAck()) {
                  delay(1000);
                }
                delay(2000);
               Radio.sendReliable(0xFF, "pm2.5:");
                while (Radio.waitingForAck()) {
                  delay(1000);
                }
                delay(2000);                
                Radio.sendReliable(0xFF, (String)pm2_5);
                while (Radio.waitingForAck()) {
                  delay(1000);
                }
                delay(2000);
               Radio.sendReliable(0xFF, "pm10:");
                while (Radio.waitingForAck()) {
                  delay(1000);
                }
                delay(2000);                
                Radio.sendReliable(0xFF, (String)pm10);
                while (Radio.waitingForAck()) {
                  delay(1000);
                }
               delay(2000); 
               Radio.sendReliable(0xFF, "#");
                while (Radio.waitingForAck()) {
                  delay(1000);
                } 
                delay(2000);
            }
            main_status = 0;    
            cur_rx_data = 0;
            pre_rx_data = 0; 
            check_sum = 0;
            for(int i=0;i< 256; i++)
            {
              recv_buff[i] = 0x00;
            }
            recv_buff_index = 0x00;      
        }
        break;
        case 4:
        break;
        case 5:
        break;
        case 6:
        break;
        case 7:
        break;
        case 8:
        break;
        case 9:
        break;
        case 10:
        break;
        case 11:
        break;
         case 12:
        break;
        case 13:
        break;
     } 
    }
//  Read_Sensordata();
  delay(500);
  digitalWrite(13,LOW);
  delay(500);
}


//void Read_Sensordata()
//{
//  uint8_t i=0;
//  Wire.requestFrom(address, 32);
//  while (Wire.available()) { // slave may send less than requested
//    Sensor_Databufer[i++] = Wire.read(); // receive a byte as character
//            
//  }
//  Serial.println("Sensor_Data:  ");
//  for(i=0;i<32;i++)
//  {
//    Serial.print(i);
//    Serial.print(":  ");
//    Serial.println(Sensor_Databufer[i],HEX); 
//    }
//  }

