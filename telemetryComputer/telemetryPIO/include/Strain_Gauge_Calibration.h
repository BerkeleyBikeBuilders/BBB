// Strain Gauge Calibration
#ifndef STRAIN_GAUGE_CALIBRATION_H
#define STRAIN_GAUGE_CALIBRATION_H

#include <Arduino.h>

// Constants and Globals
const int strainPin1 = 35;
const int strainPin2 = 36;
const int strainPin3 = 39;
const int numberStrainGauges = 3;
const double calibrationConstant = 100.0; // Update with actual calibration value
double baselineStrain[numberStrainGauges] = {0.0, 0.0, 0.0}; // Holds zeroed strain values

// Strain gauge pins array for easier iteration
const int strainPins[numberStrainGauges] = {strainPin1, strainPin2, strainPin3};

/**
 * @brief Reads the current strain gauge output.
 * 
 * @param pin Analog pin connected to the strain gauge.
 * @return double Raw strain gauge output as voltage or strain unit.
 */
double readStrainGauge(int pin) {
    return analogRead(pin) * (3.3 / 1023.0); // Adjust scaling as needed
}

/**
 * @brief Sets the baseline strain to zero by averaging multiple readings.
 */
void tare() {
    const int sampleCount = 10; // Number of readings to average

    Serial.println("Taring Strain Gauges");

    for (int i = 0; i < numberStrainGauges; i++) {
        double sum = 0.0;

        // Take multiple readings to stabilize the baseline
        for (int j = 0; j < sampleCount; j++) {
            sum += readStrainGauge(strainPins[i]);
            delay(50); // Delay between readings
        }

        // Calculate average baseline value
        baselineStrain[i] = sum / sampleCount;

        Serial.print("Baseline (zeroed) strain for gauge ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(baselineStrain[i]);
    }
}

/**
 * @brief Converts the adjusted strain gauge output to stress.
 * 
 * @param strainGaugeOutput Raw strain gauge output.
 * @param baseline Baseline strain value.
 * @return double Adjusted stress value after calibration.
 */
double convertStrainToStress(double strainGaugeOutput, double baseline) {
    return calibrationConstant * (strainGaugeOutput - baseline);
}

#endif
