#ifndef LED_H
#define LED_H

#include <Arduino.h>

typedef enum {
  RED,
  GREEN,
  BLUE,
  ORANGE,
  YELLOW,
  BLACK, //legacy, use off() or fadeDown instead
  WHITE
} Colour;     

class LED {

  private:

  int rPin;
  int gPin;
  int bPin;

  //the actual colour channels:
  int r = 255;
  int g = 255;
  int b = 255;
  float l = 1.0; // led 'lightness' (b for 'brightness' was taken so idk)

  Colour colour = WHITE;    

  //-------------------------Helper functions to shorten the code-----------------------
    int* helperGetRGB(Colour colour) {
      int red;
      int blue;
      int green;

      switch(colour) {
        case RED:
          //red = 235; green = 77; blue = 61; 
          red = 255; green = 10; blue = 0;
          break;
        case BLUE:
          //red = 40; green = 95; blue = 244; 
          red = 0; green = 47; blue = 255;
          break;
        case YELLOW:
          red = 255; green = 255; blue = 0; 
          //red = 255; green = 100; blue = 0;
          break;
        case ORANGE:
          //red = 240; green = 148; blue = 54; 
          red = 255; green = 100; blue = 0;
          break;
        case GREEN:
          //red = 101; green = 196; blue = 102; 
          red = 130; green = 255; blue = 0;
          break;
        case BLACK:
          red = 0; green = 0; blue = 0;
          break;
        case WHITE:
        default:
          red = 255; green = 255; blue = 255;
          break;
      }

      int* rgbValue = new int[3];
      rgbValue[0] = red;
      rgbValue[1] = green;
      rgbValue[2] = blue;
      return rgbValue;
    }
    
    void helperColourFade(int time, int finalRed, int finalGreen, int finalBlue) {
      int redDiff = abs(finalRed - r);
      int greenDiff = abs(finalGreen - g);
      int blueDiff = abs(finalBlue - b);

      if (redDiff == 0 && greenDiff == 0 && blueDiff == 0) {
        return;
      }

      const int dr = (finalRed - r) / redDiff;
      const int dg = (finalGreen - g) / greenDiff;
      const int db = (finalBlue - b) / blueDiff;

      int dt = round(time / max({redDiff, greenDiff, blueDiff})) + 1;

      while (max({redDiff, greenDiff, blueDiff}) > 0) {
        if (redDiff > 0) {
          r += dr;
          redDiff -= 1;
        }
        if (greenDiff > 0) {
          g += dg;
          greenDiff -= 1;
        }
        if (blueDiff > 0) {
          b += db;
          blueDiff -= 1;
        }
        on();
        delay(dt);
      }
    }
  //------------------------------------------------------------------------------------


  public:   

    void create(const int redPIN, const int greenPIN, const int bluePIN) {
      /**
      DESCRIPTION:
      stores the state in the LED instance.

      PAREMETERS:
      'redPIN': red pin for LED.
      'greenPIN': green pin for LED.
      'bluePIN': blue pin for LED.
      */

      // assign the pins
      rPin = redPIN;
      gPin = greenPIN;
      bPin = bluePIN;

      // make them output pins
      pinMode(redPIN, OUTPUT);
      pinMode(greenPIN, OUTPUT);
      pinMode(bluePIN, OUTPUT);
    }

    float getBrightness() {
      return l;
    }

    //----------------set methods-----------------
    void set(Colour setColour) {
      /**
      DESCRIPTION:
      sets the LED object to store the state. 
      Note: this is an overloaded function.

      PAREMETERS:
      'setColor': Colour enum value.
      */
      int* colour = helperGetRGB(setColour);
      r = colour[0];
      g = colour[1];
      b = colour[2];
    }

    void set(int red, int green, int blue) {
      /**
      DESCRIPTION:
      Sets the LED object to custom RGB colours, and stores the state.
      Note: this is an overloaded function.

      PAREMETERS:
      'red' 0 - 255.
      'green' 0 - 255.
      'blue' 0 - 255.
      */
      r = red;
      g = green;
      b = blue;
    }
    //--------------------------------------------

    void setBrightness(float brightness) {
      l = brightness;
    }

    void on() {
      /**
      DESCRIPTION:
      turn the LED on but doesn't change the LED settings
      */
      analogWrite(rPin, round(r * l));
      analogWrite(gPin, round(g * l));
      analogWrite(bPin, round(b * l));
    }

    void off() {
      /**
      DESCRIPTION:
      turn the LED off, but doesn't change the LED settings
      */
      analogWrite(rPin, 0);
      analogWrite(gPin, 0);
      analogWrite(bPin, 0);
    }

    void fadeDown(int time = 1000, float brightness = 0.0) {
      /**
      DESCRIPTION:
      Fades 'down' the LED to the desired brightness in (roughly) in the chosen time.

      Note: the function compensates for the starting brightness, so it always fades out in time 'time'.

      PARAMETERS:
      'time': (Optional) in miliseconds; defaults to 1000 miliseconds.
      'brightness': (Optional) the final brightness you want to stop at; defaults to 0.0.
      */

      if(brightness < l) {
          int rounded_time = abs(round(time / 100 / (l - brightness)));

          while (l >= brightness) {
              on();
              l -= 0.01;
              delay(rounded_time);
          }
          l = brightness;
      }
        
    }

    void fadeUp(int time = 1000, float brightness = 1.0) {
        /**
        DESCRIPTION:
        Fades 'up' the LED in (roughly) in the chosen time.
        
        Note: the function compensates for the starting brightness, so it always fades in in time 'time'.

        PARAMETERS:
        'time': (Optional) in miliseconds; defaults to 1000 miliseconds.
        'brightness': (Optional) the final brightness you want to stop at; defaults to 1.0.
        */


        if(brightness > l) {
          int rounded_time = abs(round(time / 100 / (l - brightness)));

          while (l <= brightness) {
              on();
              l += 0.01;
              delay(rounded_time);
          }
          l = brightness;
        }
    }
    
    void blink(int time = 1000, int num = 3, bool fade = false) {
      /**
      DESCRIPTION:
      blinks the led, with the option of fading effects.

      PARAMTERS:
      'time': (Optional) duration of the blinking; defaults to 1000 miliseconds.
      'num': (Optional) the number of blinkds in the duration; defaults to 3.
      'fade': (Optional) selects the fading option; defaults to false.
      */

      if (fade == false) {
        // if the fade option is not chosen, blink normally:
        
        // blinks takes up 15% of the time.
        int rate = round(time / num * 0.85) + 1;
        int blink = round(time / num * 0.15) + 1;

        // blinks 'num' times.
        int _ = 0;
        while(_ < num) {
          off();
          delay(blink);
          on();
          delay(rate);
          _ += 1;
        }
      } else {
        // if the fade option is true, do this:
        int rate = round(time / num * 0.60) + 1;
        int blink = round(time / num * 0.40) + 1;

        float saved_brightness = float(l);

        int _ = 0;
        while(_ < num) {
          fadeDown(blink);
          fadeUp(blink, saved_brightness);
          delay(rate - blink);
          _ += 1;
        }
      }

    }

    //----------------------------colourFade methods-------------------------------------
    void colourFade(int time, Colour colour) {
      /**
      DESCRIPTION:
      cross-fade to a different colour.
      Note: this is an overloaded function.

      PARAMTERS:
      'time': duration of the cross-fade (in miliseconds).
      'colour': the colour you want to fade to.
      */
      int* finalColour = helperGetRGB(colour);
      int finalRed = finalColour[0];
      int finalGreen = finalColour[1];
      int finalBlue = finalColour[2];

      helperColourFade(time, finalRed, finalGreen, finalBlue);
    }

    void colourFade(int time, int finalRed, int finalGreen, int finalBlue) {
      /**
      DESCRIPTION:
      cross-fade to a different colour.
      Note: this is an overloaded function.

      PARAMTERS:
      'time': duration of the cross-fade (in miliseconds).
      'finalRed': Red channel of target colour.
      'finalGreen': Green channel of target colour.
      'finalBlue': Blue channel of target colour.
      */
      helperColourFade(time, finalRed, finalGreen, finalBlue);
    }
    //-----------------------------------------------------------------------------------
};

#endif