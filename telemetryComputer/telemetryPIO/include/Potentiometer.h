#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "SD_ReadWrite.h"


class LPOT {

    int pin;
    int maxVoltage = 4095;
    int zeroVoltage = 0;
    float length; // the total length of touch surface in mm
    float V_per_mm;
    int id; // to uniquely identify the potentiometer

  public:
    void create(int P, float L, int ZEROVOLTAGE = 0, int MAXVOLTAGE = 4095, int resistanceID = 0) {
      /**
      DESCRIPTION:
      Stores the state in the LPOT linear potentiometer instance.

      PARAMTERS:
      'P': the pin for measuring
      'L': length of the potentiometer (in mm)
      'zeroVoltage': (Optional) the voltage when the potentiometer is at minimum length
      'maxVoltage': (Optional) the voltage when the potentiometer is at max length
      'resistanceID': (Optional) the code to uniquely identify the potentiometer based on the voltage value from the ID pin
      */

      pin = P;
      pinMode(pin, INPUT);

      id = resistanceID;

      length = L;
      maxVoltage = MAXVOLTAGE;
      zeroVoltage = ZEROVOLTAGE;

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

    int compareTo(LPOT linPot) {
      /**
      DESCRIPTION:
      returns negative if it's smaller, returns positive if it's larger, returns 0 if they're identical.
      */
      return id - linPot.id;
    }

    int getID() {
      return id;
    }
};

#endif