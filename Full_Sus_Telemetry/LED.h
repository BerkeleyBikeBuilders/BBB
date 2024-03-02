#ifndef LED_H
#define LED_H

class LED {

  int rPin;
  int gPin;
  int bPin;

  char colour = 'w';
  int r = 255;   // red value
  int g = 255;   // green value
  int b = 255;   // blue value
  float l = 1.0; // 'lightness' idk
  
  public:            

    void create(const int RPIN, const int GPIN, const int BPIN) {
      /**
      DESCRIPTION:
      stores the state in the LED instance.

      PAREMETERS:
      'RPIN': red pin for LED.
      'GPIN': green pin for LED.
      'BPIN': blue pin for LED.
      */

      // assign the pins
      rPin = RPIN;
      gPin = GPIN;
      bPin = BPIN;

      // make them output pins
      pinMode(RPIN, OUTPUT);
      pinMode(GPIN, OUTPUT);
      pinMode(BPIN, OUTPUT);
    }

    float getBrightness() {
      return l;
    }

    void set(char COLOUR) {
      /**
      DESCRIPTION:
      sets the LED object to store the state.

      PAREMETERS:
      'COLOUR': 'r' = red, 'g' = green 'b' = blue,
                'o' = orange, 'y' = yellow, 
                'k' = black, 'w' = white
      */

      colour = COLOUR;
      
      switch(COLOUR) {
            case 'r':  // red
              r = 255; g = 0; b = 0;
              break;
              
            case 'b':  // blue
              r = 0; g = 47; b = 255;
              break;

            case 'y':  // yellow
              r = 255; g = 100; b = 0;
              break;

            case 'o':  // orange
              r = 255; g = 40; b = 0;
              break;

            case 'g':  // green
              r = 0; g = 255; b = 0;
              break;

            case 'k':  // black (off)
              r = 0; g = 0; b = 0;
              break;
            
            default: /// white or default, which is also white
              r = 255; g = 255; b = 255;
              break;
        }
    }

    void setBrightness(float light) {
      l = light;
    }

    void ON() {
      /**
      DESCRIPTION:
      turn the LED on but doesn't change the LED settings
      */
      analogWrite(rPin, round(r * l));
      analogWrite(gPin, round(g * l));
      analogWrite(bPin, round(b * l));
    }

    void OFF() {
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
              ON();
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
              ON();
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

        // repeats 'num' times.
        int _ = 0;
        while(_ < num) {
          OFF();
          delay(blink);
          ON();
          delay(rate);
          _ += 1;
        }
      }

      else {
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

    void colourFade(int time, char colour) {
      /**
      DESCRIPTION:
      cross-fade to a different colour.

      PARAMTERS:
      'time': duration of the cross-fade (in miliseconds).
      'colour': the colour you want to fade to.
      */

      int r_final;
      int g_final;
      int b_final;

      switch(colour) {
            case 'r':  // red
              r_final = 255; g_final = 0; b_final = 0;
              break;
              
            case 'b':  // blue
              r_final = 0; g_final = 47; b_final = 255;
              break;

            case 'y':  // yellow
              r_final = 255; g_final = 100; b_final = 0;
              break;

            case 'o':  // orange
              r_final = 255; g_final = 40; b_final = 0;
              break;

            case 'g':  // green
              r_final = 0; g_final = 255; b_final = 0;
              break;

            case 'k':  // black (off)
              r_final = 0; g_final = 0; b_final = 0;
              break;
            
            default: // do nothing if the colour is invalid
              return;
          }

      int r_diff = abs(r_final - r);
      int g_diff = abs(g_final - g);
      int b_diff = abs(b_final - b);

      if (r_diff == 0 && g_diff == 0 && b_diff == 0) {
        return;
      }

      const int dr = (r_final - r) / r_diff;
      const int dg = (g_final - g) / g_diff;
      const int db = (b_final - b) / b_diff;

      int dt = round(time / max({r_diff, g_diff, b_diff})) + 1;

      while (max({r_diff, g_diff, b_diff}) > 0) {
        if (r_diff > 0) {
          r += dr;
          r_diff -= 1;
        }
        if (g_diff > 0) {
          g += dg;
          g_diff -= 1;
        }
        if (b_diff > 0) {
          b += db;
          b_diff -= 1;
        }
        ON();
        delay(dt);
      }
    }
};

#endif