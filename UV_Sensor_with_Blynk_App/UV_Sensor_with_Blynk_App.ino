
/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT Serial

#include <Wire.h> // I2C 
#include <WiFi.h> 
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Adafruit_VEML6075.h"

Adafruit_VEML6075 uv = Adafruit_VEML6075();


//Set your Auth Token here
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
#define SET_AUTH_TOKEN "a3dcbb1be625418e92fde69e3d917255";

// Your WiFi credentials.
// Set password to "" for open networks.
#define SET_SSID "NOA Labs (2.4GHz)";
#define SET_PASS "noa-labs.com";

/*DO NOT CHANGE THIS CODE HERE*/
char auth[] = SET_AUTH_TOKEN;
char ssid[] = SET_SSID;
char pass[] = SET_PASS;

BlynkTimer timer;// This function sends Arduino's up time every second to the Virtual Pins


void setup() {
 
  Serial.begin(115200);
  Serial.println("VEML6075 Full Test");
  if (! uv.begin()) {
    Serial.println("Failed to communicate with VEML6075 sensor, check wiring?");
  }
  Serial.println("Found VEML6075 sensor");
/*
  // Set the integration constant
  uv.setIntegrationTime(VEML6075_100MS);
  // Get the integration constant and print it!
  Serial.print("Integration time set to ");
  switch (uv.getIntegrationTime()) {
    case VEML6075_50MS: Serial.print("50"); break;
    case VEML6075_100MS: Serial.print("100"); break;
    case VEML6075_200MS: Serial.print("200"); break;
    case VEML6075_400MS: Serial.print("400"); break;
    case VEML6075_800MS: Serial.print("800"); break;
  }
  Serial.println("ms");

  // Set the high dynamic mode
  uv.setHighDynamic(false);
  // Get the mode
  if (uv.getHighDynamic()) {
    Serial.println("High dynamic reading mode");
  } else {
    Serial.println("Normal dynamic reading mode");
  }

  // Set the mode
  uv.setForcedMode(false);
  // Get the mode
  if (uv.getForcedMode()) {
    Serial.println("Forced reading mode");
  } else {
    Serial.println("Continuous reading mode");
  }

  // Set the calibration coefficients
  uv.setCoefficients(2.22, 1.33,  // UVA_A and UVA_B coefficients
                     2.95, 1.74,  // UVB_C and UVB_D coefficients
                     0.001461, 0.002591); // UVA and UVB responses
   */
   //Set the Blynk
   Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
 // the VEML6075's begin function can take no parameters
  // It will return true on success or false on failure to communicate

  timer.setInterval(1000L, sendData);
}

void loop() {
     Blynk.run();
     timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  Serial.print("Raw UVA reading:  "); Serial.println(uv.readUVA());
  Serial.print("Raw UVB reading:  "); Serial.println(uv.readUVB());
  Serial.print("UV Index reading: "); Serial.println(uv.readUVI());
  
}
//Display Data to Blynk
void sendData(){
   Blynk.virtualWrite(V0, uv.readUVA());
   Blynk.virtualWrite(V1, uv.readUVB());
   Blynk.virtualWrite(V2, uv.readUVI());
}

