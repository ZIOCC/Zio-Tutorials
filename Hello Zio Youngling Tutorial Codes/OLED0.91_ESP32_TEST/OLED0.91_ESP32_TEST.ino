#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#define OLED_RESET 4
//Adafruit_SSD1306 display(OLED_RESET);
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  display.clearDisplay();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  delay(100);
  // text display tests
  textDisplay();
  // draw scrolling text
  testscrolltext();
  delay(5000);
}

void textDisplay(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello,Zio Youngling!");//Type your text you want to display here
  display.display();
  display.clearDisplay();
  delay(1500);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(12,0);
  display.clearDisplay();
  display.println("Hello,Zio Youngling!");
  display.display();
  delay(1000);

 //Scroll text to the left
  display.startscrollleft(0x00, 0x0F);
  delay(1000);
  display.stopscroll();
}
