#include <Arduino.h>
#include "Strain_Gauge_Calibration.h"

void setup() {
    Serial.begin(115200);
    // variables and setting up are done in the header file!

    // Initialize the tare function to zero the strain gauge at the start
    tare();
}

void loop() {
    // Continuously read and adjust the strain gauge output based on baseline
    double currentStrain = readStrainGauge();
    double adjustedStress = convertStrainToStress(currentStrain);

    // Display the adjusted stress value
    Serial.print("Adjusted Stress: ");
    Serial.print(adjustedStress);
    Serial.println(" N");

    delay(100); // Adjust delay as needed for data sampling frequency
}