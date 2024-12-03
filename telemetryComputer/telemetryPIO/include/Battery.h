#ifndef BATT_CHECK_H
#define BATT_CHECK_H

#include <Arduino.h>
#include "LED.h"

class BatteryVoltageSensor {
  private:
    double voltage_factor = 0.0;       // To adjust for the resistors in the voltage reading
    double r1 = 464.0;                // The resistor from Battery to GPIO (a part of the voltage divider)
    double r2 = 464.0;                // The resistor from GPIO to GND (a part of the voltage divider)
    double rinternal = 1388 * 1000;   // The internal resistance from the connection between the GPIO and GND
    int battery_pin;

  public:
    /**
     * @brief Initialises the voltage-divider sensor.
     * 
     * @param resistor1 The resistor connected in series with the battery (Ω).
     * @param resistor2 The resistor connected in parallel with the board's GPIO to the GND terminal (Ω).
     * @param resistor_internal The internal impedance from the GPIO to the GND terminal (measured to be 1.388MΩ).
     */
    void create(int battery_pin, float resistor1 = 464.0, float resistor2 = 464.0, float resistor_internal = 1388.0 * 1000.0) {
      battery_pin = battery_pin;
      pinMode(battery_pin, INPUT);
      r1 = resistor1;
      r2 = resistor2;
      rinternal = resistor_internal;
      voltage_factor = adjust_voltage(r1, r2, rinternal);
    }
    
    /**
     * @brief Function to calculate the voltage adjustment factor.
     * 
     * @param resistor1 The resistor from Battery to GPIO (Ω).
     * @param resistor2 The resistor from GPIO to GND (Ω).
     * @param resistor_internal The internal impedance from the GPIO to GND (measured to be 1.388MΩ).
     * 
     * @return The voltage adjustment factor.
     */
    double adjust_voltage(double resistor1, double resistor2, double resistor_internal) {
      float r_parallel = (resistor_internal * resistor2) / (resistor_internal + resistor2);
      voltage_factor = ((r_parallel + resistor1) / r_parallel) * 0.0008058608; // 0.0008058608 converts the analogRead value into voltage within readable range (3.3/4095).
      return voltage_factor;
    }

    /**
     * @brief Gets the voltage adjustment factor.
     * 
     * @return The voltage adjustment factor.
     */
    double get_voltage_factor() {
      return voltage_factor;
    }

    /**
     * @brief Measures the battery voltage while accounting for the internal impedance of the I/O pin.
     * 
     * @param factor The adjustment factor to correct the voltage readings.
     * @param battery_pin The input pin from which the voltage is being read.
     * 
     * @return The corrected battery voltage.
     */
    float read_voltage() {
      float voltage = analogRead(battery_pin);
      float corrected_voltage = voltage * voltage_factor;
      return corrected_voltage;
    }

    /**
     * @brief Lights up a chosen LED to display the battery health.
     * 
     * @param led The LED instance to be used.
     * @param voltage The voltage reading of the battery.
     * @param warning_only If true, only shows battery status when it is low.
     */
    void display_battery(LED &led, bool warning_only = false) {
      float init_brightness = led.getBrightness();
      led.setBrightness(0);
      
      float voltage = read_voltage();
      if (voltage > 3.1) {
          if (warning_only) {return;} // Displays nothing if 'warning_only' is true
          led.set(GREEN);

      } else if (voltage >= 2.8 && voltage <= 3.1) {
          led.set(ORANGE);

      } else {
          led.set(RED);
      }
      
      led.fadeUp(200, init_brightness);
      delay(900);
      led.fadeDown(300);
      led.off();
      led.setBrightness(init_brightness);
      delay(200);
  }
};

#endif
