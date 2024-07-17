#ifndef BATT_CHECK_H
#define BATT_CHECK_H

// SETUP
#include "LED.h"
// SETUP

float batteryFactor(float R1 = 464, float R2 = 464, float Rinternal = 1388 * 1000) {
  /**
  DESCRIPTION:
  creates the factor for adjusting the reading voltage to the actual voltage.

  PARAMETERS:
  'R1': resistor connected in series with battery (ohms)
  'R2': resistor connected in parallel with the board's I/O -> GND terminal (ohms)
  'Rinternal': internal impedance through the I/O -> GND terminal (ohms)
  */

  float Rparallel = (Rinternal * R2) / (Rinternal + R2);
  return ((Rparallel + R1) / Rparallel) * 0.0008058608; // 0.0008058608 converts the analogRead value into voltage within readable range (3.3/4095).
}

float readVoltage(float factor, int batteryPin) {
  /**
  DESCRIPTION:
  measures the battery voltage while accounting for the
  internal impedance of the I/O pin.

  PARAMETERS:
  'factor': to adjust the voltage readings to the actual voltage.
  'batteryPin': the input pin you are reading from.
  */

  float voltage = analogRead(batteryPin);
  float corrected_voltage = voltage * factor;

  return corrected_voltage;
}

void displayBattery(LED &led, float voltage, bool WarningOnly = false) {
  /**
  DESCRIPTION:
  lights up a chosen LED to display the battery health.

  PARAMETERS:
  'led': the LED instance you want to use.
  'voltage': the voltage reading of the battery.
  'WarningOnly': only shows battery status when it is low.
  */

  float ogBrightness = led.getBrightness();

  if (3.1 < voltage) {
    if (WarningOnly) {
      // displays nothing if 'WarningOnly' is true
      return;
    }
    led.set('g');
  }

  else if ((2.8 <= voltage) && (voltage <= 3.1)) {
    led.set('o');
  } else {
    led.set('r');
  }

  led.setBrightness(0);
  led.fadeUp(200);
  delay(900);
  led.fadeDown(300);

  led.setBrightness(ogBrightness);
}

#endif