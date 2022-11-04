#include <Arduino.h>  //Arduino Libary
#include <FastLED.h>  //Fasled Libary
#include "OTA.h"      // Over the Air configuration
#include "FireAndWorms.h" //file to include the two funtcions Fire or Worms


#define NUM_LEDS  119 //Number of LED's in the Strip
#define LED_PIN   2   //Pin on The ESP 32


void setup() {        //Setup to flash the Chip, is called once
     
  FastLED.addLeds<WS2812B, LED_PIN, RGB>(leds, NUM_LEDS);            // Add our LED strip to the FastLED library
  FastLED.setBrightness(255);

  Serial.begin(115200); //sets the data  Rate on Serial  0=No Data
 
  ArduinoOTA.setHostname("leds"); //Hostname to show when looking for OTA 
  setupOTA("leds", mySSID, myPASSWORD); //Set up OTA (Hostname,mySSID and myPASSWORD defined in the credentials.h file)
  setup_routing(); //calls function setup_routing above
}



void loop() { //Loop to Flash
  
  ArduinoOTA.handle(); //OTA function needs to get called once in a while
  
  server.handleClient(); // handelt de kliänt
    
    if (isFire == 1) { // if variable "isFire" set to 0 via port 80 on 192.168.1.22/Worms this code runs until elif below
      
      FirePattern(); // calls the function FirePattern in the file WormsPatternAndSlow.h
      
      server.handleClient(); // handelt de kliänt  
    } 

    else if (isFire == 0) {     //if variable isFire is set to 1 this code runs instead of Worms above
      
      WormsPattern();

      server.handleClient(); // handelt de kliänt // calls the function "Fire" in the file WormsPatternAandSlow.
    }
}