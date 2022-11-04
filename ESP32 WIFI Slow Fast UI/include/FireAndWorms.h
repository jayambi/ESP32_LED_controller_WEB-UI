#include <Arduino.h>  //Arduino Libary
#include <FastLED.h>  //Fastled Libary
#include "fire.h"
#include "html.h"

#define NUM_LEDS  119 //Number of LED's in the Strip
#define LED_PIN   2   //Pin on The ESP 32

extern bool isFire;


CRGB leds[NUM_LEDS];  //Defines the RGB Pixel 

uint8_t hue  = 0;
uint8_t hue2 = 60;
uint8_t hue3 = 101;
uint8_t hue4 = 160;
uint8_t hue5 = 210;
uint8_t hue6 = 255; //variables for Colour-start point in Rainbow worms effect

void FirePattern() { //whenever the button "fast" is clicked on 192.168.1.22 (leds.local) this code runs in void loop in main.cpp"
 

  ArduinoOTA.handle(); //OTA function needs to get called once in a while

  //FireEffectSmooth fire(NUM_LEDS, true, false, 1, 1.0, 1.0, 1.0);
  //(NUM_LEDS, mirrored = true, reversed = false, sparkHeight, sparkProbability, cooling, spreadRate)  
  //(int size, bool mirrored = true, bool reversed = false, int sparkHeight = 0, float sparkProbability = 1.0, float cooling = 1.0, float spreadRate = 1.0)
   
   ClassicFireEffect fire(NUM_LEDS, 5, 1, 2, 1, false, true);
  //ClassicFireEffect(int size, int cooling = 80, int sparking = 50, int sparks = 3, int sparkHeight = 4, bool breversed = true, bool bmirrored = true)   
  
  


        while(isFire == 1){
            
            FastLED.clear();
            fire.DrawFire();
            FastLED.show();  //  Show and delay    

            ArduinoOTA.handle(); //OTA function needs to get called once in a whil
            server.handleClient();
                
                if(isFire == 0)
                    break;

        }

};

void WormsPattern() {  //Whenever the button "slow" is toggled on 192.168.1.22 this code runs in the void loop  in main.cpp
  
    ArduinoOTA.handle(); //OTA function needs to get called once in a while

    uint16_t sinBeat   = beatsin16(3, 0, NUM_LEDS - 1, 0, 0);
    uint16_t sinBeat2  = beatsin16(5, 0, NUM_LEDS - 1, 0, 21845);
    uint16_t sinBeat3  = beatsin16(4, 0, NUM_LEDS - 1, 0, 43690);
    /* uint16_t sinBeat4  = beatsin16(9, 0, NUM_LEDS - 1, 0, 10000);
    uint16_t sinBeat5  = beatsin16(6, 0, NUM_LEDS - 1, 0, 30845);
    uint16_t sinBeat6  = beatsin16(7, 0, NUM_LEDS - 1, 0, 55690);  *///betsin16 worms slow
    
    leds[sinBeat]   = CHSV(hue,  255, 255);
    leds[sinBeat2]  = CHSV(hue2, 255, 255);
    leds[sinBeat3]  = CHSV(hue3, 255, 255);
    /* leds[sinBeat4]  = CHSV(hue4, 255, 255);
    leds[sinBeat5]  = CHSV(hue5, 255, 255);
    leds[sinBeat6]  = CHSV(hue6, 255, 255);  */// defines the colour in each betsin16 changing in worms slow

    EVERY_N_MILLISECONDS(30){
    hue++;
    } 
    
    EVERY_N_MILLISECONDS(19){
    hue2++;
    }

    EVERY_N_MILLISECONDS(18){
    hue3++;
    }

    EVERY_N_MILLISECONDS(40){
    hue4++;
    }
    
    EVERY_N_MILLISECONDS(50){
    hue5++;
    }
    
    EVERY_N_MILLISECONDS(60){
    hue6++;
    }                           //increases hue every n milliseconds in slow
    
    EVERY_N_MILLISECONDS(1){
    for(int i = 0; i < 4; i++) {
        blur1d(leds, NUM_LEDS, 100);
    }
    }                           // blurs the LEDS out in the beatsin16 function in slow
    
    fadeToBlackBy(leds, NUM_LEDS,5);

    FastLED.show();
};

   

