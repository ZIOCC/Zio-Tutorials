/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 16 servos, one after the other

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815

  These displays use I2C to communicate, 2 pins are required to  
  interface. For Arduino UNOs, thats SCL -> Analog 5, SDA -> Analog 4

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
  
  PCA9685  --  TB6612       PCA9685  --   TB6612
  0        --  PWMA1         8       --   A2IN1
  1        --  A1IN1         9       --   A2IN2
  2        --  A1IN2         10      --   STBY2
  3        --  STBY1         11      --   B2IN1
  4        --  B1IN1         12      --   B2IN2
  5        --  B1IN2         13      --   PWMB2
  6        --  PWMB1         14      --   IO14
  7        --  PWMA2         15      --   IO15

 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  1000 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  3000 // this is the 'maximum' pulse length count (out of 4096)

// our servo # counter
//uint8_t servonum = 0;
//uint8_t i = 0;
uint16_t pulselen = 1000;

void setup() {
  Serial.begin(9600);
  Serial.println("4 channel Motor test!");

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  yield();
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
//void setServoPulse(uint8_t n, double pulse) {
//  double pulselength;
//  
//  pulselength = 1000000;   // 1,000,000 us per second
//  pulselength /= 60;   // 60 Hz
//  Serial.print(pulselength); Serial.println(" us per period"); 
//  pulselength /= 4096;  // 12 bits of resolution
//  Serial.print(pulselength); Serial.println(" us per bit"); 
//  pulse *= 1000;
//  pulse /= pulselength;
//  Serial.println(pulse);
//  pwm.setPWM(n, 0, pulse);
//}

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
  delay(2000);

    for(pulselen=1000;pulselen<4096;pulselen+=100)
  {
    goback();
    pwm.setPWM(0, 0, pulselen);
    pwm.setPWM(6, 0, pulselen);
    pwm.setPWM(7, 0, pulselen);
    pwm.setPWM(13, 0, pulselen);
    delay(100);
    }
  delay(500);
  stp();
  delay(2000);
}

