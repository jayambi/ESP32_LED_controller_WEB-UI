//+--------------------------------------------------------------------------
//
// NightDriver - (c) 2020 Dave Plummer.  All Rights Reserved.
//
// File:        fire.h               
//
// Description:
//
//              LED Flame Effect
//
// History:     Sep-28-2020     davepl      Created
//
//---------------------------------------------------------------------------

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "ledgfx.h"



CHSV HeatColor_custom( uint8_t temperature)
{
    CHSV heatcolor_custom;

    // Scale 'heat' down from 0-255 to 0-191,
    // which can then be easily divided into three
    // equal 'thirds' of 64 units each.
    uint8_t t192 = scale8_video( temperature, 191);

    // calculate a value that ramps up from
    // zero to 255 in each 'third' of the scale.
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252

    // now figure out which third of the spectrum we're in:
    if( t192 & 0x80) {
        // we're in the hottest third
        heatcolor_custom.h = heatramp; // full red
        heatcolor_custom.s = 255; // full green
        heatcolor_custom.v = 255; // ramp up blue

    } else if( t192 & 0x40 ) {
        // we're in the middle third
        heatcolor_custom.h = heatramp; // full red
        heatcolor_custom.s = 255; // ramp up green
        heatcolor_custom.v = 200; // no blue

    } else {
        // we're in the coolest third
        heatcolor_custom.h = heatramp; // ramp up red
        heatcolor_custom.s = 255; // no green
        heatcolor_custom.v = heatramp; // no blue
    }

    return heatcolor_custom;
}



class ClassicFireEffect
{
protected:
    int     Size;
    int     Cooling;
    int     Sparks;
    int     SparkHeight;
    int     Sparking;
    bool    bReversed;
    bool    bMirrored;

    byte  * heat;

    // When diffusing the fire upwards, these control how much to blend in from the cells below (ie: downward neighbors)
    // You can tune these coefficients to control how quickly and smoothly the fire spreads.  

    static const byte BlendSelf = 10;
    static const byte BlendNeighbor1 = 10;
    static const byte BlendNeighbor2 = 2;
    static const byte BlendNeighbor3 = 2;
    static const byte BlendTotal = (BlendSelf + BlendNeighbor1 + BlendNeighbor2 + BlendNeighbor3);

public:
    
    // Lower sparking -> more flicker.  Higher sparking -> more consistent flame

    ClassicFireEffect(int size, int cooling = 80, int sparking = 50, int sparks = 3, int sparkHeight = 4, bool breversed = true, bool bmirrored = true) 
        : Size(size),
          Cooling(cooling),
          Sparks(sparks),
          SparkHeight(sparkHeight),
          Sparking(sparking),
          bReversed(breversed),
          bMirrored(bmirrored)
    {
        if (bMirrored)
            Size = Size / 2;

        heat = new byte[size] { 0 };
    }

    virtual ~ClassicFireEffect()
    {
        delete [] heat;
    }

    virtual void DrawFire()
    {
            EVERY_N_MILLISECONDS(30){
    
             // First cool each cell by a little bit
             for (int i = 0; i < Size; i++) 
                heat[i] = max(0L, heat[i] - random(0, ((Cooling * 10) / Size) + 2));
            }

            EVERY_N_MILLISECONDS(30){
                                    
             // Next drift heat up and diffuse it a little but
             for (int i = 0; i < Size; i++)
                heat[i] = (heat[i] * BlendSelf + 
                       heat[(i + 1) % Size] * BlendNeighbor1 + 
                       heat[(i + 2) % Size] * BlendNeighbor2 + 
                       heat[(i + 3) % Size] * BlendNeighbor3) 
                      / BlendTotal;

            }


            // Randomly ignite new sparks down in the flame kernel
            for (int i = 0; i < Sparks; i++)
            {
                if (random(50) < Sparking)
            {
                int y = Size - 1 - random(SparkHeight);
                heat[y] = heat[y] + random(50, 200); // This randomly rolls over sometimes of course, and that's essential to the effect
            }
        }

        // Finally convert heat to a color
        for (int i = 0; i < Size; i++)
        {
            CRGB color = HeatColor_custom(heat[i]);
            int j = bReversed ? (Size - 1 - i) : i;
            DrawPixels(j, 1, color);
            if (bMirrored)
            {
                int j2 = !bReversed ? (2 * Size - 1 - i) : Size + i;
                DrawPixels(j2, 1, color);
            }
        }
    }
};