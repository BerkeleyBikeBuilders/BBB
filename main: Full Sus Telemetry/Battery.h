#ifndef BATT_CHECK_H
#define BATT_CHECK_H

//SETUP
#include "LED.h"
//SETUP


float battery_factor(float R1 = 464, float R2 = 464, float Rinternal = 1388 * 1000){
  /**
  DESCRIPTION:
  creates the factor for adjusting the reading voltage to the actual voltage.

  PARAMETERS:
  'R1': resistor connected in series with battery (ohms)
  'R2': resistor connected in parallel with the board's I/O -> GND terminal (ohms)
  'Rinternal': internal impedance through the I/O -> GND terminal (ohms)
  */

  float Rparallel = (Rinternal * R2) / (Rinternal + R2);
  return ((Rparallel + R1) / Rparallel) * 0.0008058608; //0.0008058608 converts the analogRead value into voltage within readable range (3.3/4095).
}

float vibe_check(float factor, int batt_pin){
  /**
  DESCRIPTION:
  measures the battery voltage while accounting for the
  internal impedance of the I/O pin.

  PARAMETERS:
  'factor': to adjust the voltage readings to the actual voltage.
  'batt_pin': the input pin you are reading from.
  */

  float voltage = analogRead(batt_pin);
  float corrected_voltage = voltage * factor;

  return corrected_voltage;
}

void battery_display(LED &led, float voltage){
  /**
  DESCRIPTION:
  lights up a chosen LED to display the battery health.

  PARAMETERS:
  'led': the LED instance you want to use.
  'voltage': the voltage reading of the battery.
  */

  float og_brightness = led.l;

  if (voltage < 2.8){
    led.set('r');
  }

  if ((2.8 <= voltage) && (voltage <= 3.1)){
    led.set('o');
  }

  if (3.1 < voltage){
    led.set('g');
  }

  led.set_brightness(0);
  led.fade_up(200);
  delay(800);
  led.fade_down(300);

  led.set_brightness(og_brightness);
}

#endif