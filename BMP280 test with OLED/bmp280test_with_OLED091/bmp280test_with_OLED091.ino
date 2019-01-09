/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Zio Qwiic BMEP280
  ----> https://www.smart-prototyping.com/Zio-Qwiic-Air-Pressure-Sensor-BMP280

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
//Adafruit_SSD1306 display = Adafruit_SSD1306();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_BMP280 bmp; // I2C

void setup() {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done

  // Clear the buffer.
  display.clearDisplay();
  
  Serial.println(F("BMP280 test"));
  display.print(F("BMP280 test"));

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    display.print(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);

   display.display();
   
  // Clear the buffer.
  display.clearDisplay();
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

void loop() {

    //OLED Display
  
    //Set the font and text
    display.setTextSize(1);
    display.setTextColor(WHITE);

  //Temperature
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    display.setCursor(0,0);
    display.print(F("Temp = "));
    display.print(bmp.readTemperature());
    display.println(" *C");


  //Pressure
    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    display.setCursor(0,10);
    display.print(F("Pres = "));
    display.print(bmp.readPressure());
    display.println(" Pa");


  //Altitude
    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude()); /* Adjusted to local forecast! */
    Serial.println(" m");

    display.setCursor(0,20);
    display.print(F("Alt = "));
    display.print(bmp.readAltitude()); /* Adjusted to local forecast! */
    display.println(" m");

    Serial.println();
    display.display();
    delay(1000);
    // Clear the buffer.
    display.clearDisplay();
}
