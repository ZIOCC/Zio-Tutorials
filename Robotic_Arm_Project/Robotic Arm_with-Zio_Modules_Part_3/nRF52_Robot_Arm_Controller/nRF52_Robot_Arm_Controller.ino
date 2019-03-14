/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <bluefruit.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

BLEUart bleuart;
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Function prototypes for packetparser.cpp
uint8_t readPacket (BLEUart *ble_uart, uint16_t timeout);
float   parsefloat (uint8_t *buffer);
void    printHex   (const uint8_t * data, const uint32_t numBytes);

// Packet buffer
extern uint8_t packetbuffer[];

#define SERVOMIN0  350 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX0  542 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN1  300 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX1  550 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN2  350 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX2  600 // this is the 'maximum' pulse length count (out of 4096)
#define SERVOMIN3  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX3  350 // this is the 'maximum' pulse length count (out of 4096)
#define SERVO     0x40
// our servo # counter
uint8_t servonum0 = 0,servonum1 = 1,servonum2 = 2,servonum3 = 3;
int pulselen0=550,pulselen1=550,pulselen2=600,pulselen3=350;


void setup(void)
{
  Serial.begin(115200);
  //while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println(F("Adafruit Bluefruit52 Controller App Example"));
  Serial.println(F("-------------------------------------------"));

  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Bluefruit52");

  // Configure and start the BLE Uart service
  bleuart.begin();

  // Set up and start advertising
  startAdv();
  bleuart.setRxCallback(read_data);
  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println(); 
  pwm.begin(SERVO);
  pwm.setPWMFreq(60,SERVO);  // Analog servos run at ~60 Hz updates 
}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  
  // Include the BLE UART (AKA 'NUS') 128-bit UUID
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}
void read_data()
{
  uint8_t len = readPacket(&bleuart, 500);
  if (len == 0) return;
  }
/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  // Wait for new data to arrive
//  uint8_t len = readPacket(&bleuart, 10);
//  if (len == 0) return;

  // Got a packet!
  // printHex(packetbuffer, len);


  // Buttons
  if (packetbuffer[1] == 'B') {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';
    Serial.print ("Button "); Serial.print(buttnum);
    if (pressed) {
      Serial.println(" pressed");
      switch(buttnum)
      {
        case 1: pulselen0++;break;
        case 2: pulselen0--;break;
        case 3: pulselen1++;break;
        case 4: pulselen1--;break;
        case 5: pulselen2++;break;
        case 6: pulselen2--;break;
        case 7: pulselen3++;break;
        case 8: pulselen3--;break;
        default:break;
        }
    } else {
      Serial.println(" released");
    }
  }
  if(pulselen0>SERVOMAX0) 
  {pulselen0=SERVOMAX0;}
  if(pulselen0<SERVOMIN0) 
  {pulselen0=SERVOMIN0;}
  
  if(pulselen1>SERVOMAX1) 
  {pulselen1=SERVOMAX1;}
  if(pulselen1<SERVOMIN1) 
  {pulselen1=SERVOMIN1;}

  if(pulselen2>SERVOMAX2) 
  {pulselen2=SERVOMAX2;}
  if(pulselen2<SERVOMIN2) 
  {pulselen2=SERVOMIN2;}
  
  if(pulselen3>SERVOMAX3) 
  {pulselen3=SERVOMAX3;}
  if(pulselen3<SERVOMIN3) 
  {pulselen3=SERVOMIN3;}
  
  pwm.setPWM(servonum0, 0, pulselen0,SERVO);
  pwm.setPWM(servonum1, 0, pulselen1,SERVO);
  pwm.setPWM(servonum2, 0, pulselen2,SERVO);
  pwm.setPWM(servonum3, 0, pulselen3,SERVO);
  Serial.print(" pulselen0:  ");
  Serial.println(pulselen0);
  Serial.print(" pulselen1:  ");
  Serial.println(pulselen1);
  Serial.print(" pulselen2:  ");
  Serial.println(pulselen2);
  Serial.print(" pulselen3:  ");
  Serial.println(pulselen3);
  delay(10);
}
