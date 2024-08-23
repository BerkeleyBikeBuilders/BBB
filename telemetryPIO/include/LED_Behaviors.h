#ifndef LED_BEHAVIOURS_H
#define LED_BEHAVIOURS_H

#include <Arduino.h>
#include "LED.h"
#include "Buttons.h"

/**
 * @brief Starts the recording process by fading up the LED in green.
 * 
 * @param led The LED instance to control.
 */
void startRecording(LED &led) {
  float initBrightness = led.getBrightness();

  led.set(GREEN);
  led.setBrightness(0.0);
  led.fadeUp(300, initBrightness);

  led.setBrightness(initBrightness);
}

/**
 * @brief Stops the recording process by fading down the LED in red.
 * 
 * @param led The LED instance to control.
 */
void stop_recording(LED &led) {
  float initBrightness = led.getBrightness();

  led.changeColour(280, RED);
  led.fadeDown(200);

  led.setBrightness(initBrightness);
  led.off();
}

/**
 * @brief Resumes the recording process by changing the LED color to green.
 * 
 * @param led The LED instance to control.
 */
void resume_recording(LED &led) {
  led.changeColour(280, GREEN);
}

/**
 * @brief Pauses the recording process by changing the LED color to yellow.
 * 
 * @param led The LED instance to control.
 */
void pause_recording(LED &led) {
  led.changeColour(280, YELLOW);
}

/**
 * @brief Displays an idle animation by breathing the LED on/off in white.
 * 
 * @param led The LED instance to control.
 */
void idle(LED &led) {
  float initBrightness = led.getBrightness();

  led.setBrightness(0.5);

  led.set(WHITE);
  led.fadeUp(1000);
  led.fadeDown(1000);

  led.setBrightness(initBrightness);
}

/**
 * @brief Shows a "thinking" animation by blinking the LED on/off in white.
 * 
 * @param led The LED instance to control.
 */
void thinking(LED &led) {
  led.set(WHITE);
  led.fadeUp(500);
  led.fadeDown(1000);
}

/**
 * @brief Confirms an action by briefly fading up and then down the LED in green.
 * 
 * @param led The LED instance to control.
 */
void confirm(LED &led) {
  float initBrightness = led.getBrightness();

  led.setBrightness(0.0);
  led.set(GREEN);
  led.fadeUp(100, initBrightness);
  delay(100);
  led.fadeDown(300);
  led.setBrightness(initBrightness);
}

/**
 * @brief Shows a "sleep" animation by gradually reducing the LED brightness while blinking.
 * 
 * @param led The LED instance to control.
 */
void sleep(LED &led) {
  int initBrightness = led.getBrightness();

  led.off();
  led.set(WHITE);

  led.fadeUp(8000);
  led.fadeDown(1000);
  led.fadeUp(800, 0.5);
  led.fadeDown(1000);

  led.off();
  led.setBrightness(initBrightness);
}

#endif