#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Appfruits_QTouch.h"
#include "TouchWheel.h"

uint16_t volume = 10;
uint16_t vol1 = 0;
uint16_t vol2 = 0;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
byte mp3Address = 0x37; //Unshifted 7-bit default address for Qwiic MP3
Appfruits_QTouch qtouch = Appfruits_QTouch();
TouchWheel touchWheel(&qtouch);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  qtouch.begin();
  touchWheel.begin();
  pinMode(13, OUTPUT);
  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("OLED FeatherWing test");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
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
  displaymag();
  while (qtouch.eventAvailable())
  {
    Serial.println("TOUCHING...");
    //display which key is pressed or not, 1 = pressed and 0 = not pressed
    for (uint8_t i = 3; i < 12; i++) //from key 3 to key 12
    {
      Serial.print("Key");
      Serial.print(i);
      Serial.print(": " );
      Serial.println(qtouch.isKeyPressed(i));
      if (qtouch.isKeyPressed(i) == 1) { //if a key is pressed, play the song corresponding to the key number
        mp3ChangeVolume(volume);
        mp3PlayTrack(i - 2);
        displaymag();
        delay(1000);
      }
    }
    digitalWrite(13, HIGH);
    touchWheel.update(); //sliding on the wheel
    uint8_t P = touchWheel.getSliderPosition(); //for gettting the position of the finger on the wheel
    Serial.println("TOUCHING...");
    Serial.println("P");
    Serial.println(P);
    delay(100);
    touchWheel.update();
    uint8_t P2 = touchWheel.getSliderPosition();
    Serial.println("TOUCHING...");
    Serial.println("P2");
    Serial.println(P2);
    if (P2 > P) { //if we slide our finger to the clock side, volume up
      volume++;
      mp3ChangeVolume(volume);
      displaymag();
    }
    else if (P2 < P) { //if we side in the other side, volume down
      volume--;
      mp3ChangeVolume(volume);
      displaymag();
    }
    delay(100);
  }
  digitalWrite(13, LOW);

}

void displaymag() { //displaying informations in the screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Music played : ");
  String songName = mp3SongName(); //for printing the song name
  display.println(songName);
  display.print("Volume : "); //showing the volume level
  display.println(volume);
  display.display();
}
