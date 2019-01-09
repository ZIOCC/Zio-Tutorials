/*************************************************** 
  This is an example for the SHT31 Zio Qwiic Temperature & Humidity (SHT31) Sensor

  Designed specifically to work with the SHT31 sensor
  ----> https://www.smart-prototyping.com/Zio-Qwiic-Temperature-Humidity-Sensor-SHT31

  These sensors use I2C to communicate, 2 pins are required to  
  interface
 ****************************************************/
 
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin();  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done

  // Clear the buffer.
  display.clearDisplay();
  
  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("SHT31 test");
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}


void loop() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

    //OLED Display
  
    //Set the font and text
    display.setTextSize(1);
    display.setTextColor(WHITE);

  if (! isnan(t)) {  // check if 'is not a number'
    display.setCursor(0,0);
    Serial.print("Temp *C = "); Serial.println(t);
    display.print("Temp *C = "); display.println(t);
  } else { 
    display.setCursor(0,0);
    Serial.println("Failed to read temperature");
    display.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    display.setCursor(0,10);
    Serial.print("Hum. % = "); Serial.println(h);
    display.print("Hum. % = "); display.println(h);
  } else { 
    display.setCursor(0,18);
    Serial.println("Failed to read humidity");
    display.println("Failed to read humidity");
  }
  Serial.println();
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
}
