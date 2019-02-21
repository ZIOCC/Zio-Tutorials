#include <Wire.h> // I2C 
#include <WiFi.h> 
#include <WiFiClient.h>
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp32.h> 

const char* ssid     = "Your SSID";
const char* password = "password";
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Auth Token";

IRTherm therm; // Create an IRTherm object to interact with throughout

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BLYNK_PRINT Serial
BlynkTimer timer;// This function sends Arduino's up time every second to the Virtual Pins

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() 
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, password);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  therm.begin(); // Initialize thermal IR sensor
  therm.setUnit(TEMP_F); // Set the library's units to Farenheit
  // Alternatively, TEMP_F can be replaced with TEMP_C for Celsius orTEMP_K for Kelvin.
  timer.setInterval(1000L, sendSensor);
}

void loop() {
 // Call therm.read() to read object and ambient temperatures from the sensor.
  if (therm.read()) // On success, read() will return 1, on fail 0.
  {
    printTobject();
    printTobjecTval();
    unit1();
    printTambient();
    printTambienTval();
    unit2();
    delay(500);
  }
   display.display();
   Blynk.run();
   timer.run();
}
//Display data to OLED
void printTobject() 
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5,0);
  display.println("Object: ");
}

void printTobjecTval() 
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(60,0);
  display.println(therm.object());
}

void unit1() 
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(100,0);
  display.println("F");
}

void printTambient() 
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5,20);
  display.println("Ambient:");
}

void printTambienTval() 
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(60,20);
  display.println(therm.ambient());
}
void unit2() 
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(100,20);
  display.println("F");
}
//Display data to Blynk
void sendSensor()
{
  Blynk.virtualWrite(V0,therm.object());
  Blynk.virtualWrite(V1, therm.ambient());
  }
