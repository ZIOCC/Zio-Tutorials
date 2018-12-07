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
#define SOIL_MOISTURE


//Set your Auth Token here
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
#define SET_AUTH_TOKEN "";

// Your WiFi credentials.
// Set password to "" for open networks.
#define SET_SSID "";
#define SET_PASS "";

/*DO NOT CHANGE THIS CODE HERE*/
char auth[] = SET_AUTH_TOKEN;
char ssid[] = SET_SSID;
char pass[] = SET_PASS;

// This function sends Arduino's up time every second to the Virtual Pins
BlynkTimer timer;


void setup() {
 // Debug console
  Serial.begin(9600);
  
  //Set the Blynk
   Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  
  // put your additional setup code here, to run once:
  /* You can copy existing code examples provided by 
   * your sample code of your board in this section*/

  // Here is where the data is being sent from hardware to Blynk
  timer.setInterval(1000L,sendStatus);

}


void loop() {
 
     Blynk.run();
     timer.run();
  /* You can inject your own code or combine it with other sketches.
   * Check other examples on how to communicate with Blynk. Remember
   * to avoid delay() function!*/
  // put your main code here, to run repeatedly: 
   
}


//Here is where the data is being written and then displayed to Blynk App
void sendStatus()
{
  // You can send any value at any time.
  // Please don't send more than 10 values per second.
  //You can add more based on your virtual pins
  Blynk.virtualWrite(V1, value);//change the 'value' aacordingly
}
