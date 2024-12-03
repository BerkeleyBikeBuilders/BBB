#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "SD_ReadWrite.h"

/**
 * @brief Class representing a linear potentiometer (LPOT).
 *        Used to measure position along a linear touch-sensitive surface.
 */
class LPOT {
    int pin;                       // Pin connected to the potentiometer
    int maxVoltage = 4095;         // Maximum voltage when at full length
    int zeroVoltage = 0;           // Voltage when at minimum length
    float length;                  // Total length of the potentiometer in mm
    float V_per_mm;                // Voltage change per mm of length
    int id;                        // Unique identifier for the potentiometer

public:
    /**
     * @brief Initializes the LPOT instance with given parameters.
     *
     * @param P The pin connected to the potentiometer.
     * @param L The length of the potentiometer in mm.
     * @param ZEROVOLTAGE Optional: Voltage at minimum length (default is 0).
     * @param MAXVOLTAGE Optional: Voltage at maximum length (default is 4095).
     * @param resistanceID Optional: Unique identifier for the potentiometer (default is 0).
     */
    void create(int P, float L, int ZEROVOLTAGE = 0, int MAXVOLTAGE = 4095, int resistanceID = 0) {
        pin = P;
        pinMode(pin, INPUT);

        id = resistanceID;
        length = L;
        maxVoltage = MAXVOLTAGE;
        zeroVoltage = ZEROVOLTAGE;

        V_per_mm = (maxVoltage - zeroVoltage) / length;  // Calculate voltage per mm
    }

    /**
     * @brief Reads the position from the potentiometer and converts it to a length in mm.
     *
     * @return The position along the potentiometer's length in mm.
     */
    float read() {
        int detectedV = analogRead(pin);
        return (detectedV - zeroVoltage) / V_per_mm;  // Convert voltage to length
    }

    /**
     * @brief Resets the zeroVoltage to the current voltage reading.
     *        Useful for recalibrating the potentiometer.
     */
    void zero() {
        zeroVoltage = analogRead(pin);
    }

    /**
     * @brief Compares this potentiometer's ID to another potentiometer's ID.
     *
     * @param linPot The potentiometer to compare against.
     * @return A negative value if this ID is smaller, positive if larger, or 0 if identical.
     */
    int compareTo(LPOT linPot) {
        return id - linPot.id;
    }

    /**
     * @brief Retrieves the unique ID of this potentiometer.
     *
     * @return The unique ID of the potentiometer.
     */
    int getID() {
        return id;
    }
};

#endif
