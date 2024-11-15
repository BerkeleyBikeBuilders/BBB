// Strain Gauge Calibration
#ifndef STRAIN_GAUGE_CALIBRATION_H
#define STRAIN_GAUGE_CALIBRATION_H

#include <Arduino.h>

double baselineStrain[3] = {0.0, 0.0, 0.0}; // Variable to hold the zeroed strain value
double calibrationConstant = 100.0; // Example calibration constant (update with actual calibration)
const int strainPin1 = 35; // Analog pin connected to the strain gauge (I have no clue what the pinnings are lol)
const int strainPin2 = 36; // What pin's do we want the other strain gauges on?
const int strainPin3 = 39;

const int numberStrainGauges = 3;

/**
 * @brief Reads the current strain gauge output.
 * 
 * @return double - The raw strain gauge output as a voltage or strain unit.
 */

double readStrainGauge(int Pin) {
    // For example, reading from an analog pin and scaling to a strain unit
    return analogRead(Pin) * (3.3 / 1023.0); // Adjust as needed(what are we using?)
}

/**
 * @brief Sets the baseline strain to zero before a run by (averaging) multiple readings.
 */

void tare() {

    const int sampleCount = 10; // Number of readings to average (can adjust)

    Serial.println("Taring Strain Gauges");

    const int strainPins[] = {strainPin1, strainPin2, strainPin3};

    // Take multiple readings to get a stable baseline
    for (int i = 0; i < numberStrainGauges; i++) {
        double sum = 0.0;
        for (int j = 0; j < sampleCount; j++) {
            sum += readStrainGauge(strainPins[i]);
            delay(50); // Short delay between readings to stabilize
        }
    
    // Calculate the average baseline value
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
 * @param strainGaugeOutput - The raw strain gauge output.
 * @return double - The adjusted stress value after calibration.
 */

double convertStrainToStress(double strainGaugeOutput, double baseline) {
    return calibrationConstant * (strainGaugeOutput - baseline); // Adjusted strain
}

#endif