#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SLAVE_BROADCAST_ADDR 0x00  //default address
#define SLAVE_ADDR 0x00       //SLAVE_ADDR 0xA0-0xAF
uint16_t distance_H=0;
uint16_t distance_L=0;
uint16_t distance=0;

// Set Counter 
uint16_t time_sit1 = 0;//
uint16_t time_sit2 = 0; 

uint16_t time_leave1 = 0;
uint16_t time_leave2 = 0;


uint16_t lim = 75; //Distance range from sensor to the seat
uint16_t maxsit_time = 7200000; //Set the maximum sitting time in ms



Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.println("OLED FeatherWing test");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  Serial.println("OLED begun");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  Serial.println("IO test");
}

void loop() {    
  Wire.beginTransmission(SLAVE_ADDR); // transmit to device #8
  Wire.write(1);              // measure command: 0x01
  Wire.endTransmission();    // stop transmitting 
  Wire.requestFrom(SLAVE_ADDR, 2);    // request 6 bytes from slave device #8
  
  while (Wire.available()) { // slave may send less than requested

  if(distance*0.1 < lim){// detects if a person is within the detection range
    distance_H = Wire.read();  
    distance_L = Wire.read();  
    distance = (uint16_t)distance_H<<8;
    distance = distance|distance_L;
    sit();  
    time_leave1++; //tracks the time nobody is around
    calculatetime();
    
    if(time_sit2 > maxsit_time){
      maxsit();
      time_leave1 = millis()/1000;
      time_leave1++;
      calculatetime();
    }
    else if (distance*0.1 > lim){//detects if a person is out of range
      calculatetime();
      Serial.print("Time sit : ");
      Serial.print(time_sit2/1000);  
      Serial.println(" sec");  
      time_sit1 = millis()/1000;
      Serial.println("Nobody");
      time_sit1++;
      delay(1000);
      
      break;
    }
  }
  
  else if(distance*0.1 > lim){//detects if a person is out of range   
    Serial.println("Nobody");
    time_sit1++;
    delay(1000);  
    distance_H = Wire.read();
    distance_L = Wire.read();  
    distance = (uint16_t)distance_H<<8;
    distance = distance|distance_L;   
    
    if(distance*0.1 < lim){ // detects if a person is within the detection range   
      calculateleave();
      time_leave1 = millis()/1000;
      sit();
      time_leave1++;    
      calculatetime();
    
    if(time_sit2 > maxsit_time){
      maxsit();
      time_leave1 = millis()/1000;
      time_leave1++;
      calculatetime();
    }
     else if (distance*0.1 > lim){
      calculatetime();    
      Serial.print("Time sit : ");
      Serial.print(time_sit2/1000);
      Serial.println(" sec");
      time_sit1 = millis()/1000;
      time_sit1++;
      delay(1000);
    }
    }
  }
  
  } 
 }

 void sit(){ //calculate the distance from the ultrasonic sensor
  Serial.print("Sit "); 
  Serial.print(distance*0.1);
  Serial.println("cm"); 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println();
  display.println("Sit distance : ");
  display.print(distance*0.1);
  display.println(" cm"); 
  display.display();
  delay(1000);
  return;
 }

 void maxsit(){ //when the user sit for a long time
  Serial.println("");
  Serial.println("Take a break !");
  delay(1000);
  if(distance*0.1>lim){
    time_sit1 = millis()/1000; // tracks the time a person not sitting
    time_sit2 = 0;//set sitting counter to zero again
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Take a break !");
  display.display();
  delay(1000);
  return;
 }
 
 void calculatetime(){ //calculate for how long the user sits
  time_sit2 = millis()-(time_sit1*1000);  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Time sit : ");
  display.print(time_sit2/1000);
  display.println(" sec"); 
  display.display();
  return;
 }

 void calculateleave(){ //calculate for how long the user left the workspace
  time_leave2 = millis()- (time_leave1*1000);
  Serial.print("Time leave : ");
  Serial.print(time_leave2/1000);
  Serial.println(" sec");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Time leave : ");
  display.print(time_leave2/1000);
  display.println(" sec"); 
  display.display();
  delay(1000);
  return;
 }
