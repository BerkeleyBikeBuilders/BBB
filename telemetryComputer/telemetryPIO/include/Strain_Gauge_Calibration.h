// Strain Gauge Calibration
#ifndef STRAIN_GAUGE_CALIBRATION_H
#define STRAIN_GAUGE_CALIBRATION_H

#include <Arduino.h>

double baselineStrain = 0.0; // Variable to hold the zeroed strain value
double calibrationConstant = 1000.0; // Example calibration constant (update with actual calibration)
const int strainPin = A0; // Analog pin connected to the strain gauge (this pin? not 100% on pinning)

/**
 * @brief Reads the current strain gauge output.
 * 
 * @return double - The raw strain gauge output as a voltage or strain unit.
 */

double readStrainGauge() {
    // For example, reading from an analog pin and scaling to a strain unit
    return analogRead(strainPin) * (5.0 / 1023.0); // Adjust as needed(what are we using?)
}

/**
 * @brief Sets the baseline strain to zero before a run by (averaging) multiple readings.
 */

void tare() {
    double sum = 0.0;
    const int sampleCount = 10; // Number of readings to average (can adjust)

    Serial.println("Taring...");

    // Take multiple readings to get a stable baseline
    for (int i = 0; i < sampleCount; i++) {
        sum += readStrainGauge();
        delay(50); // Short delay between readings to stabilize
    }

    // Calculate the average baseline value
    baselineStrain = sum / sampleCount;

    Serial.print("Baseline (zeroed) strain: ");
    Serial.println(baselineStrain);
}

/**
 * @brief Converts the adjusted strain gauge output to stress.
 * 
 * @param strainGaugeOutput - The raw strain gauge output.
 * @return double - The adjusted stress value after calibration.
 */

double convertStrainToStress(double strainGaugeOutput) {
    return calibrationConstant * (strainGaugeOutput - baselineStrain); // Adjusted strain
}

#endif