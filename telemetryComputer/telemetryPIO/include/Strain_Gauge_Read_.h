// Strain Gauge Read (Write?)
#ifndef STRAIN_GAUGE_READ_H
#define STRAIN_GAUGE_READ_H

#include <Arduino.h>

class STRAIN_GAUGE {
    int pin;
    int maxVoltage;
    int zeroVoltage;
    float sensitivityCoefficient;
    int attachedResistance;

public:
    /**
     * @brief Initializes the strain gauge with the specified parameters.
     * 
     * @param PIN Analog pin connected to the strain gauge.
     * @param ATTACHED_R Attached resistance in ohms (default: 221).
     * @param ZEROVOLTAGE Initial zero voltage (default: 0).
     * @param MAXVOLTAGE Maximum voltage reading (default: 4095).
     * @param SENSITIVITY_COEFF Sensitivity coefficient (default: 2.1).
     */
    void create(int PIN, int ATTACHED_R = 221, int ZEROVOLTAGE = 0, int MAXVOLTAGE = 4095, float SENSITIVITY_COEFF = 2.1) {
        pin = PIN;
        pinMode(pin, INPUT);
        attachedResistance = ATTACHED_R;
        zeroVoltage = ZEROVOLTAGE;
        maxVoltage = MAXVOLTAGE;
        sensitivityCoefficient = SENSITIVITY_COEFF;
    }

    /**
     * @brief Reads and returns the voltage adjusted for zeroing.
     * 
     * @return float Adjusted voltage value.
     */
    float read() {
        int detectedVoltage = analogRead(pin);
        return detectedVoltage - zeroVoltage;
    }

    /**
     * @brief Zeros the minimum voltage to the current reading.
     */
    void zero() {
        zeroVoltage = analogRead(pin);
    }
};

class STRAIN_GAUGES {
    STRAIN_GAUGE* strainGauges = nullptr;
    int sensorCount = 0;
    int initializedSensors = 0;

public:
    /**
     * @brief Allocates memory for the specified number of strain gauges.
     * 
     * @param sensorNumber Number of strain gauges to manage.
     */
    void create(int sensorNumber) {
        sensorCount = sensorNumber;
        strainGauges = (STRAIN_GAUGE*)malloc(sensorNumber * sizeof(STRAIN_GAUGE));

        if (!strainGauges) {
            Serial.println("Memory allocation failed!");
            return;
        }
    }

    /**
     * @brief Adds a new strain gauge with the specified parameters.
     * 
     * @param PIN Analog pin connected to the strain gauge.
     * @param ATTACHED_R Attached resistance in ohms (default: 221).
     * @param ZEROVOLTAGE Initial zero voltage (default: 0).
     * @param MAXVOLTAGE Maximum voltage reading (default: 4095).
     * @param SENSITIVITY_COEFF Sensitivity coefficient (default: 2.1).
     */
    void addGauge(int PIN, int ATTACHED_R = 221, int ZEROVOLTAGE = 0, int MAXVOLTAGE = 4095, float SENSITIVITY_COEFF = 2.1) {
        if (initializedSensors >= sensorCount) {
            Serial.println("Cannot add more gauges: Maximum limit reached.");
            return;
        }

        strainGauges[initializedSensors].create(PIN, ATTACHED_R, ZEROVOLTAGE, MAXVOLTAGE, SENSITIVITY_COEFF);
        initializedSensors++;
    }
};

#endif
