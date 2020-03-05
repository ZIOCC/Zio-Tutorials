/* This is a standard Blynk App template code to be used with Zio or any Arduino compatible products
 * Works with the Zio Zuino M Uno and Zuino PsyFi32 Development Board
 * ----> https://www.smart-prototyping.com/Zuino-M-UNO.html
 * ----> https://www.smart-prototyping.com/Zuino-XS-PsyFi32.html
 *
 *
 * This code works using PUSH (using Virtual pins as the variables)
 * Compiled, Edited and Written by Zalifah Aswana for Smart Prototying
*/


/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************/

/*************************************************************
  Output any data on LCD widget!
  App project setup:
    LCD widget, switch to ADVANCED mode, select pin V1
 *************************************************************/

 
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

//Add additional libraries here
#include <Wire.h> // I2C 
#include <WiFi.h> 
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h> 
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  1000 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  3000 // this is the 'maximum' pulse length count (out of 4096)
#define MOTOR 0x40

//define for soil moisture sensor
#define COMMAND_LED_OFF     0x00
#define COMMAND_LED_ON      0x01
#define COMMAND_GET_VALUE        0x05
#define COMMAND_NOTHING_NEW   0x99


Adafruit_PWMServoDriver pwm;
const byte qwiicAddress = 0x28;     //Default Address for soil moisture sensor
uint16_t ADC_VALUE= 0;


//Set your Auth Token here
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
#define SET_AUTH_TOKEN "c3dac222d5d54f2884217c7311bff293";

// Your WiFi credentials.
// Set password to "" for open networks.
#define SET_SSID "your-wifi-name";
#define SET_PASS "yourw-wifi-password";

/*DO NOT CHANGE THIS CODE HERE*/
char auth[] = SET_AUTH_TOKEN;
char ssid[] = SET_SSID;
char pass[] = SET_PASS;

// This function sends Arduino's up time every second to the Virtual Pins
BlynkTimer timer;
WidgetLCD lcd(V1);

/******YOU CAN MAKE CHANGES HERE**************************/

//Set your moisture level here
uint16_t DRY= 650;
uint16_t WET= 250;
uint16_t NORMAL= 500;

//Set the time needed to check plant moisture to water your plant again
uint16_t DELAY= 3600000;//1 hour in milliseconds interval

//set the time needed for your sensor to checks again and confirm the value
uint16_t CONFIRM= 900000;//15 minutes in milliseconds interval

// our servo # counter
uint16_t pulselen = 1000;

/***********END TO MAKING CHANGES**********************/


void setup() {

  // Debug console
  Serial.begin(9600);
  Serial.println("Zio Qwiic Soil Moisture Sensor Master Awake");
  //setup for soil moisture sensor
  Wire.begin();
  testForConnectivity();
  ledOn();
  delay(500);
  
  //setup for the motor driver
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  yield();
  
  //Set the Blynk
   Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  
  // put your additional setup code here, to run once:
  /* You can copy existing code examples provided by 
   * your sample code of your board in this section*/
  lcd.clear(); //Use it to clear the LCD Widget
  lcd.println(0, 0, ADC_VALUE);
  // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  // Please use timed events when LCD printintg in void loop to avoid sending too many commands
  // It will cause a FLOOD Error, and connection will be dropped
   
}

void  goahead()
{
  pwm.setPin(3,0,1);    //H  STBY1  
  pwm.setPin(1,0,1);    //H
  pwm.setPin(2,4095,1); //L
  pwm.setPin(4,0,1);    //H        
  pwm.setPin(5,4095,1); //L
    
  pwm.setPin(10,0,1);    //H  STBY2  
  pwm.setPin(8,0,1);    //H
  pwm.setPin(9,4095,1); //L
  pwm.setPin(11,0,1);    //H        
  pwm.setPin(12,4095,1); //L
  }

void  goback()
{
  pwm.setPin(3,0,1);    //H  STBY1  
  pwm.setPin(2,0,1);    //H
  pwm.setPin(1,4095,1); //L
  pwm.setPin(5,0,1);    //H        
  pwm.setPin(4,4095,1); //L
    
  pwm.setPin(10,0,1);    //H  STBY2  
  pwm.setPin(9,0,1);    //H
  pwm.setPin(8,4095,1); //L
  pwm.setPin(12,0,1);    //H        
  pwm.setPin(11,4095,1); //L
  }  

 void stp()
 {
  pwm.setPin(3,0,0);     //L  STBY1
  pwm.setPin(10,0,0);    //L  STBY2
  }


void loop() {
     
    timer.run();
    Blynk.run();
  /* You can inject your own code or combine it with other sketches.
   * Check other examples on how to communicate with Blynk. Remember
   * to avoid delay() function!*/
  // put your main code here, to run repeatedly: 
   delay(1000);
  waterPlant();
}

// LED is off, and a -1 if an error occurred.
void get_value() {
  Wire.beginTransmission(qwiicAddress);
  Wire.write(COMMAND_GET_VALUE); // command for status
  Wire.endTransmission();    // stop transmitting //this looks like it was essential.

  Wire.requestFrom(qwiicAddress, 2);    // request 1 bytes from slave device qwiicAddress

  while (Wire.available()) { // slave may send less than requested
  uint8_t ADC_VALUE_L = Wire.read(); 
  uint8_t ADC_VALUE_H = Wire.read();
  ADC_VALUE=ADC_VALUE_H;
  ADC_VALUE<<=8;
  ADC_VALUE|=ADC_VALUE_L;
  Serial.print("ADC_VALUE:  ");
  Serial.println(ADC_VALUE,DEC);
  }
  uint16_t x=Wire.read(); 
}

void ledOn() {
  Wire.beginTransmission(qwiicAddress);
  Wire.write(COMMAND_LED_ON);
  Wire.endTransmission();
}

void ledOff() {
  Wire.beginTransmission(qwiicAddress);
  Wire.write(COMMAND_LED_OFF);
  Wire.endTransmission();
}


// testForConnectivity() checks for an ACK from an Sensor. If no ACK
// program freezes and notifies user.
void testForConnectivity() {
  Wire.beginTransmission(qwiicAddress);
  //check here for an ACK from the slave, if no ACK don't allow change?
  if (Wire.endTransmission() != 0) {
    Serial.println("Check connections. No slave attached.");
    while (1);
  }
}

void warn(){
       ledOn();
       delay(100);
       ledOff();
       delay(100);
}

void pumpWater(){
  for(pulselen=1000;pulselen<4096;pulselen+=100)
  {
    goahead();
    pwm.setPWM(0, 0, pulselen);
    pwm.setPWM(6, 0, pulselen);
    pwm.setPWM(7, 0, pulselen);
    pwm.setPWM(13, 0, pulselen);
    delay(100);
    }
  delay(500);
  stp();
  delay(1000);

    for(pulselen=1000;pulselen<4096;pulselen+=100)
  {
//    goback();
    pwm.setPWM(0, 0, pulselen);
    pwm.setPWM(6, 0, pulselen);
    pwm.setPWM(7, 0, pulselen);
    pwm.setPWM(13, 0, pulselen);
    delay(100);
    }
  delay(500);
  stp();
  delay(1000);
}

void confirmValue(){
     warn();
     delay(CONFIRM);//15 minutes delay
     get_value(); //checks plant moisture again to confirm
}


void waterPlant(){

   get_value();//Checks your plant's moisture level
   
   if (ADC_VALUE >= DRY) {//set the sensor to tell you that it needs H2O
     
    confirmValue();
     
    if (ADC_VALUE >= DRY){
     pumpWater();
     }
    else {
      waterPlant();
    }
   }
   else if (ADC_VALUE < DRY & ADC_VALUE >= NORMAL) {//set the sensor to blink rapidly if it is nearly dry
     
    confirmValue();
    
    if (ADC_VALUE < DRY & ADC_VALUE >= NORMAL){
     warn();
    }
        else {
      waterPlant();
    }
   }
   else {
    delay(DELAY);
   }
}


