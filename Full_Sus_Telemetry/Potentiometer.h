#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

// SETUP
#include "SD_ReadWrite.h"
// SETUP


class LPOT {
  public:

    int pin;
    int maxVoltage = 4095;
    int zeroVoltage = 0;
    float length; //the total length of touch surface in mm
    float V_per_mm;


    void create(int P, float L, int zeroVoltage = 0, int maxVoltage = 4095) {
      /**
      DESCRIPTION:
      Stores the state in the LPOT linear potentiometer instance.

      PARAMTERS:
      'P': the pin for measuring
      'L': length of the potentiometer (in mm)
      'zeroVoltage': (Optional) the voltage when the potentiometer is at minimum length
      'maxVoltage': (Optional) the voltage when the potentiometer is at max length
      */

      pin = P;
      pinMode(pin, INPUT);

      length = L;
      maxVoltage = maxVoltage;
      zeroVoltage = zeroVoltage;

      V_per_mm = (maxVoltage - zeroVoltage) / length;
    }

    float read() {
      /**
      DESCRIPTION:
      Reads and returns the voltage from the potentiometer.
      */

      int detectedV = analogRead(pin);

      return (detectedV - zeroVoltage) / V_per_mm;
    }

    void zero() {
      /**
      DESCRIPTION:
      Zeros the minimum voltage; this won't affect the V_per_mm.
      */

      zeroVoltage = analogRead(pin);
    }
};

#endif