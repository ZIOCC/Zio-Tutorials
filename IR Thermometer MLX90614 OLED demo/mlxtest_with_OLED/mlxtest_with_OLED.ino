/*************************************************** 
  This is a library example for the Zio Qwiic Surface Temperature IR Sensor

  Designed specifically to work with the MLX90614 sensors in the
  Smart Prototyping shop
  ----> https://www.smart-prototyping.com/Zio-Qwiic-Surface-Temperature-Infrared-Sensor-MLX9061

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  
 ****************************************************/

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);

   display.begin();
  // init done

  // Clear the buffer.
  display.clearDisplay();

  //OLED Display setup
  
  //Set the font and text
  display.setTextSize(1);
  display.setTextColor(WHITE);

  
  Serial.println("Zio MLX90614 test");
  display.println("Zio MLX90614 test");    
  display.display();
  delay(3000);
  mlx.begin();

  // Clear the buffer.
  display.clearDisplay();
    
}

void loop() {
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");

  //OLED Display
  display.setCursor(0,0);
  display.print("Amb : "); display.print(mlx.readAmbientTempC());display.println("*C");
  display.setCursor(0,8);
  display.print("Obj : "); display.print(mlx.readObjectTempC()); display.println("*C");
  display.setCursor(0,16);
  display.print("Amb : "); display.print(mlx.readAmbientTempF()); display.println("*F");
  display.setCursor(0,24);
  display.print("Obj : "); display.print(mlx.readObjectTempF()); display.println("*F");

  Serial.println();
  display.display();
  delay(500);
  
  // Clear the buffer.
  display.clearDisplay();
}
