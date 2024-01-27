#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

// Dependencies:
#include "SD_ReadWrite.h"


class LPOT {
  public:

    int pin;
    int maxV = 4095;
    int zeroV = 0;
    float length; //the total length of touch surface in mm
    float V_per_mm;


    void create(int P, float L, int ZEROV = 0, int MAXV = 4095){
      /**
      DESCRIPTION:
      Stores the state in the LPOT linear potentiometer instance.

      PARAMTERS:
      'time': (Optional) duration of the blinking; defaults to 1000 miliseconds.
      'num': (Optional) the number of blinkds in the duration; defaults to 3.
      'fade': (Optional) selects the fading option; defaults to false.
      */

      pin = P;
      pinMode(pin, INPUT);

      length = L;
      maxV = MAXV;
      zeroV = ZEROV;

      V_per_mm = (maxV-zeroV)/length;
    }

    float read(){
      /**
      DESCRIPTION:
      Reads and returns the voltage from the potentiometer.
      */

      float detectedV = analogRead(pin);
      float rangeV = abs(maxV - zeroV); //abs in case you entered the values wrong

      return float((detectedV / rangeV) * length);
    }

    void zero(){
      /**
      DESCRIPTION:
      Zero the minimum voltage, and recalculate the voltage range based on it.
      */

      zeroV = analogRead(pin);
    }
};

#endif