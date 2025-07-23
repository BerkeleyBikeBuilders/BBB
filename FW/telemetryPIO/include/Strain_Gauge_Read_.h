// Strain Gauge Read (Write?)
#ifndef STRAIN_GAUGE_READ_H
#define STRAIN_GAUGE_READ_H

#include <Arduino.h>

class STRAIN_GAUGE {
    int pin;
    int maxVoltage = 4095;
    int zeroVoltage = 0;
    float sensitivityCoefficient;
    int attached_r;

  public:
    void create(int PIN, int ATTACHED_R = 221, int ZEROVOLTAGE = 0, int MAXVOLTAGE = 4095, int SENSITIVITY_COEFF = 2.1) {

      pin = PIN;
      pinMode(pin, INPUT);

      maxVoltage = MAXVOLTAGE;
      zeroVoltage = ZEROVOLTAGE;
      sensitivityCoefficient = SENSITIVITY_COEFF;
      attached_r = ATTACHED_R;
    }

    float read() {
      /**
      DESCRIPTION:
      Reads and returns the voltage from the potentiometer.
      */

      int detectedV = analogRead(pin);
      return detectedV - zeroVoltage;
    }

    void zero() {
      /**
      DESCRIPTION:
      Zeros the minimum voltage; this won't affect the V_per_mm.
      */

      zeroVoltage = analogRead(pin);
    }
};

class STRAIN_GAUGES {

    STRAIN_GAUGE* strainGauges;
    int sensorNum = 0;
    int sensorSize = 0;

    public:

    void create(int sensorNumber) {
        sensorNum = sensorNumber;
        strainGauges = (STRAIN_GAUGE*) malloc(sensorNumber * sizeof(STRAIN_GAUGE));
        if (strainGauges == NULL) {
            printf("memory allocation failed!");
            return;
        }
    }

    void addGauge(int PIN, int ATTACHED_R = 221, int ZEROVOLTAGE = 0, int MAXVOLTAGE = 4095, int SENSITIVITY_COEFF = 2.1) {
        if (sensorSize >= sensorNum) {
            return;
        } else {
            
        }
    }
};

#endif