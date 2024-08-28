#ifndef BATT_CHECK_H
#define BATT_CHECK_H

#include <Arduino.h>
#include "LED.h"

class BatteryVoltageSensor {
  private:
    double voltageFactor = 0.0;       // To adjust for the resistors in the voltage reading
    double R1 = 464.0;                // The resistor from Battery to GPIO (a part of the voltage divider)
    double R2 = 464.0;                // The resistor from GPIO to GND (a part of the voltage divider)
    double Rinternal = 1388 * 1000;   // The internal resistance from the connection between the GPIO and GND
    int batteryPin;

  public:
    /**
     * @brief Initialises the voltage-divider sensor.
     * 
     * @param Resistor1 The resistor connected in series with the battery (Ω).
     * @param Resistor2 The resistor connected in parallel with the board's GPIO to the GND terminal (Ω).
     * @param ResistorInternal The internal impedance from the GPIO to the GND terminal (measured to be 1.388MΩ).
     */
    void create(int BatteryPin, float Resistor1 = 464.0, float Resistor2 = 464.0, float ResistorInternal = 1388.0 * 1000.0) {
      batteryPin = BatteryPin;
      pinMode(batteryPin, INPUT);
      R1 = Resistor1;
      R2 = Resistor2;
      Rinternal = ResistorInternal;
      voltageFactor = adjustVoltage(R1, R2, Rinternal);
    }
    
    /**
     * @brief Function to calculate the voltage adjustment factor.
     * 
     * @param Resistor1 The resistor from Battery to GPIO (Ω).
     * @param Resistor2 The resistor from GPIO to GND (Ω).
     * @param ResistorInternal The internal impedance from the GPIO to GND (measured to be 1.388MΩ).
     * 
     * @return The voltage adjustment factor.
     */
    double adjustVoltage(double Resistor1, double Resistor2, double ResistorInternal) {
      float Rparallel = (ResistorInternal * Resistor2) / (ResistorInternal + Resistor2);
      voltageFactor = ((Rparallel + Resistor1) / Rparallel) * 0.0008058608; // 0.0008058608 converts the analogRead value into voltage within readable range (3.3/4095).
      return voltageFactor;
    }

    /**
     * @brief Gets the voltage adjustment factor.
     * 
     * @return The voltage adjustment factor.
     */
    double getVoltageFactor() {
      return voltageFactor;
    }

    /**
     * @brief Measures the battery voltage while accounting for the internal impedance of the I/O pin.
     * 
     * @param factor The adjustment factor to correct the voltage readings.
     * @param batteryPin The input pin from which the voltage is being read.
     * 
     * @return The corrected battery voltage.
     */
    float readVoltage() {
      float voltage = analogRead(batteryPin);
      float corrected_voltage = voltage * voltageFactor;
      return corrected_voltage;
    }

    /**
     * @brief Lights up a chosen LED to display the battery health.
     * 
     * @param led The LED instance to be used.
     * @param voltage The voltage reading of the battery.
     * @param warningOnly If true, only shows battery status when it is low.
     */
    void displayBattery(LED &led, bool warningOnly = false) {
      float initBrightness = led.getBrightness();
      led.setBrightness(0);
      
      float voltage = readVoltage();
      if (voltage > 3.1) {
          if (warningOnly) {return;} // Displays nothing if 'warningOnly' is true
          led.set(GREEN);

      } else if (voltage >= 2.8 && voltage <= 3.1) {
          led.set(ORANGE);

      } else {
          led.set(RED);
      }
      
      led.fadeUp(200, initBrightness);
      delay(900);
      led.fadeDown(300);
      led.off();
      led.setBrightness(initBrightness);
      delay(200);
  }
};

#endif