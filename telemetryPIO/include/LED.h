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
    float l = 1.0F; // led 'lightness' (b for 'brightness' was taken so idk)
    float peakl = 1.0F;


    //--------------------------------Helper functions start----------------------------------
    struct RGB {
      int red;
      int green;
      int blue;
    };

    /**
     * @brief Convert a Colour object to RGB values.
     *
     * @param colour The colour to convert. Options: RED, GREEN, BLUE, ORANGE, YELLOW, BLACK, WHITE.
     * @return A struct representing the RGB values.
     */
    RGB getColourRGB(Colour colour) {
        switch (colour) {
        //                       r     g     b
            case RED:    return {255,  10,   0}; //{235, 77, 61}
            case GREEN:  return {90,   255,  0}; //{40, 95, 244}
            case BLUE:   return {0,    47,   255};
            case ORANGE: return {255,  100,  0}; //{240, 148, 54}
            case YELLOW: return {255,  200,  0}; 
            case BLACK:  return {0,    0,    0};
            case WHITE:
            default:     return {255,  255,  255};
        }
    }
    
    /**
     * @brief Helper function to cross-fade the LED to a target colour.
     *
     * @param time Duration of the cross-fade in milliseconds.
     * @param targetRed The final red channel value.
     * @param targetGreen The final green channel value.
     * @param targetBlue The final blue channel value.
     */
    void helperChangeColour(int time, int targetRed, int targetGreen, int targetBlue) {
        int redDiff = abs(targetRed - r);
        int greenDiff = abs(targetGreen - g);
        int blueDiff = abs(targetBlue - b);

        if (redDiff == 0 && greenDiff == 0 && blueDiff == 0) {
            return;
        }

        //calculates the colour step sizes (avoids 0 division)
        int dr = (redDiff == 0) ? 0 : (targetRed - r) / redDiff;
        int dg = (greenDiff == 0) ? 0 : (targetGreen - g) / greenDiff;
        int db = (blueDiff == 0) ? 0 : (targetBlue - b) / blueDiff;

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
    //--------------------------------Helper functions end------------------------------------


  public:   
    /**
     * @brief Initialises the LED with the specified pins.
     *
     * @param redPIN Red pin for the LED.
     * @param greenPIN Green pin for the LED.
     * @param bluePIN Blue pin for the LED.
     */
    void create(int redPIN, int greenPIN, int bluePIN) {
        rPin = redPIN;
        gPin = greenPIN;
        bPin = bluePIN;

        pinMode(redPIN, OUTPUT);
        pinMode(greenPIN, OUTPUT);
        pinMode(bluePIN, OUTPUT);
    }

    /**
     * @brief Get the current brightness of the LED.
     *
     * @return The brightness as a float value.
     */
    float getBrightness() const {
        return l;
    }

    /**
     * @brief Calibrate the peak brightness of the LED.
     *
     * @param peakBrightness The peak brightness value.
     */
    void calibrateBrightness(float peakBrightness) {
        peakl = peakBrightness;
    }


    //------------------------------ Set methods start -------------------------------
    /**
     * @brief Set the LED to a preset colour.
     *
     * @param setColour The Colour enum value.
     */
    void set(Colour setColour) {
        RGB colour = getColourRGB(setColour);
        r = colour.red;
        g = colour.green;
        b = colour.blue;
    }

    /**
     * @brief Set the LED to custom RGB values.
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
    //------------------------------ Set methods end ---------------------------------


    /**
     * @brief Set the brightness of the LED.
     *
     * @param brightness The brightness as a ratio of the peak brightness.
     */
    void setBrightness(float brightness) {
        l = constrain(brightness, 0.0F, 1.0F) ;
    }

    /**
     * @brief Turn on the LED with the current settings.
     */
    void on() const {
      analogWrite(rPin, constrain(round(r * l * peakl), 0, 255));
      analogWrite(gPin, constrain(round(g * l * peakl), 0, 255));
      analogWrite(bPin, constrain(round(b * l * peakl), 0, 255));
    }

    /**
     * @brief Turn off the LED.
     */
    void off() const {
        analogWrite(rPin, 0);
        analogWrite(gPin, 0);
        analogWrite(bPin, 0);
    }


    //-------------------------------------------fade methods start------------------------------------------------
    /**
     * @brief Fades 'down' the LED to the desired brightness in the chosen time.
     *
     * @param time (Optional) Duration of the fade in milliseconds; defaults to 1000 milliseconds.
     * @param brightness (Optional) The final brightness to stop at; defaults to 0.0.
     * 
     * @note The function compensates for the starting brightness, so it always fades out in the specified time.
     */
    void fadeDown(int time, float brightness = 0.0F) {
      brightness = constrain(brightness, 0.0F, 1.0F); // apply peak brightness multiplier

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
     * @brief Fades 'up' the LED to the desired brightness in the chosen time (as a ratio of the peak brightness).
     *
     * @param brightness The final brightness to stop at.
     * @param time (Optional) Duration of the fade in milliseconds; defaults to 1000 milliseconds.
     * 
     * @note The function compensates for the starting brightness, so it always fades in within the specified time.
     */
    void fadeUp(int time, float brightness = 1.0F) {
      brightness = constrain(brightness, 0.0F, 1.0F); // apply peak brightness multiplier

      if(brightness > l) {
        int rounded_time = abs(round(time / 100 / (l - brightness)));

        while (l <= brightness) {
            on();
            l += 0.01F;
            delay(rounded_time);
        }
        l = brightness;
      }
    }
    //-------------------------------------------fade methods end----------------------------------------------------
      
    /**
     * @brief Blink the LED with optional fading effects.
     *
     * @param time Duration of the blinking in milliseconds.
     * @param num The number of blinks.
     * @param fade Whether to apply a fading effect.
     */
    void blink(int time = 1000, int num = 3, bool fade = false) {

        int blinkOn = fade ? round(time / num * 0.40f) 
                           : round(time / num * 0.15f);

        int blinkOff = fade ? round(time / num * 0.60f) 
                            : round(time / num * 0.85f);

        for (int i = 0; i < num; i++) {
            if (fade) {
                float savedBrightness = l;
                fadeDown(blinkOn);
                fadeUp(blinkOn, savedBrightness);
            } else {
                off();
                delay(blinkOff);
                on();
                delay(blinkOn);
            }
        }
    }

    //----------------------------- Change Colour methods start -------------------------------
    /**
     * @brief Cross-fade the LED to a different colour.
     *
     * @param time Duration of the cross-fade in milliseconds.
     * @param colour The target colour.
     */
    void changeColour(int time, Colour colour) {
        RGB finalColour = getColourRGB(colour);
        helperChangeColour(time, finalColour.red, finalColour.green, finalColour.blue);
    }

    /**
     * @brief Cross-fade the LED to a different colour by specifying RGB values.
     *
     * @param time Duration of the cross-fade in milliseconds.
     * @param targetRed The final red channel value.
     * @param targetGreen The final green channel value.
     * @param targetBlue The final blue channel value.
     */
    void changeColour(int time, int targetRed, int targetGreen, int targetBlue) {
        helperChangeColour(time, targetRed, targetGreen, targetBlue);
    }
    //----------------------------- Change Colour methods end --------------------------------
};

#endif