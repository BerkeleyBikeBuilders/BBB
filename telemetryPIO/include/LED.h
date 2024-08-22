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
  Colour colour = WHITE; // LED defaults to white 
  int r = 255;
  int g = 255;
  int b = 255;
  float l = 1.0; // led 'lightness' (b for 'brightness' was taken so idk)

  //--------------------------------Helper functions------------------------------------
    /**
     * @brief Converts the Colour object to RGB values.
     *
     * @return A length 3 integer array representing the RGB values.
     *
     * @param colour The colour to convert. Options are: RED, GREEN, BLUE, ORANGE, YELLOW, BLACK, WHITE.
     */
    int* getColourRGB(Colour colour) {
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
          red = 255; green = 200; blue = 0; 
          //red = 255; green = 100; blue = 0;
          break;
        case ORANGE:
          //red = 240; green = 148; blue = 54; 
          red = 255; green = 100; blue = 0;
          break;
        case GREEN:
          red = 90; green = 255; blue = 0; 
          //red = 130; green = 255; blue = 0;
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
    
    /**
     * @brief Converts the colour to RGB values with a cross-fade.
     *
     * @param time Duration of the cross-fade (in milliseconds).
     * @param targetRed The final red channel value.
     * @param targetGreen The target green channel value.
     * @param targetBlue The target blue channel value.
     */
    void helperChangeColour(int time, int targetRed, int targetGreen, int targetBlue) {
      int redDiff = abs(targetRed - r);
      int greenDiff = abs(targetGreen - g);
      int blueDiff = abs(targetBlue - b);

      if (redDiff == 0 && greenDiff == 0 && blueDiff == 0) {
        return;
      }

      const int dr = (targetRed - r) / redDiff;
      const int dg = (targetGreen - g) / greenDiff;
      const int db = (targetBlue - b) / blueDiff;

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
    
    /**
     * @brief Stores the state in the LED instance.
     *
     * @param redPIN Red pin for the LED.
     * @param greenPIN Green pin for the LED.
     * @param bluePIN Blue pin for the LED.
     */
    void create(const int redPIN, const int greenPIN, const int bluePIN) {
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
    /**
     * @brief Sets the LED object to store the state.
     * 
     * @note This is an overloaded function.
     *
     * @param setColor Colour enum value.
     */ 
    void set(Colour setColour) {
      int* colour = getColourRGB(setColour);
      r = colour[0];
      g = colour[1];
      b = colour[2];
    }
    
    /**
     * @brief Sets the LED object to custom RGB colours and stores the state.
     * 
     * @note This is an overloaded function.
     *
     * @param red Red channel value (0 - 255).
     * @param green Green channel value (0 - 255).
     * @param blue Blue channel value (0 - 255).
     */
    void set(int red, int green, int blue) {
      r = red;
      g = green;
      b = blue;
    }
    //--------------------------------------------

    /**
     * @brief sets and stores the LED brightness.
     */
    void setBrightness(float brightness) {
      l = brightness;
    }
    
    /**
     * @brief turn the LED on but doesn't change the LED settings.
     */
    void on() {
      analogWrite(rPin, round(r * l));
      analogWrite(gPin, round(g * l));
      analogWrite(bPin, round(b * l));
    }
    /**
     * @brief turn the LED off, but doesn't change the LED settings.
     */
    void off() {
      analogWrite(rPin, 0);
      analogWrite(gPin, 0);
      analogWrite(bPin, 0);
    }

    /**
     * @brief Fades 'down' the LED to the desired brightness in the chosen time.
     * 
     * @note The function compensates for the starting brightness, so it always fades out in the specified time.
     *
     * @param time (Optional) Duration of the fade in milliseconds; defaults to 1000 milliseconds.
     * @param brightness (Optional) The final brightness to stop at; defaults to 0.0.
     */
    void fadeDown(int time = 1000, float brightness = 0.0) {
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
    
    /**
     * @brief Fades 'up' the LED to the desired brightness in the chosen time.
     * 
     * @note The function compensates for the starting brightness, so it always fades in within the specified time.
     *
     * @param time (Optional) Duration of the fade in milliseconds; defaults to 1000 milliseconds.
     * @param brightness (Optional) The final brightness to stop at; defaults to 1.0.
     */
    void fadeUp(int time = 1000, float brightness = 1.0) {
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
      
    /**
     * @brief Blinks the LED with the option of fading effects.
     *
     * @param time (Optional) Duration of the blinking in milliseconds; defaults to 1000 milliseconds.
     * @param num (Optional) The number of blinks within the duration; defaults to 3.
     * @param fade (Optional) Whether to apply a fading effect; defaults to false.
     */
    void blink(int time = 1000, int num = 3, bool fade = false) {
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

    //----------------------------changeColour methods-------------------------------------
    /**
     * @brief Cross-fades the LED to a different colour.
     *
     * @param time Duration of the cross-fade in milliseconds.
     * @param colour The target colour to fade to.
    */
    void changeColour(int time, Colour colour) {
      int* finalColour = getColourRGB(colour);
      int targetRed = finalColour[0];
      int targetGreen = finalColour[1];
      int targetBlue = finalColour[2];

      helperChangeColour(time, targetRed, targetGreen, targetBlue);
    }

    /**
     * @brief Cross-fades the LED to a different colour by specifying RGB values.
     *
     * @param time Duration of the cross-fade in milliseconds.
     * @param targetRed The final red channel value.
     * @param targetGreen The final green channel value.
     * @param targetBlue The final blue channel value.
     */
    void changeColour(int time, int targetRed, int targetGreen, int targetBlue) {
      helperChangeColour(time, targetRed, targetGreen, targetBlue);
    }
    //-------------------------------------------------------------------------------------
};

#endif