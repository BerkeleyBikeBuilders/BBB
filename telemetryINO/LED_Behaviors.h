#ifndef LED_BEHAVIOURS_H
#define LED_BEHAVIOURS_H

// SETUP
#include "LED.h"
#include "Buttons.h"
// SETUP

void startRecording(LED &led) {
  float ogBrightness = led.getBrightness();

  led.set(GREEN);
  led.setBrightness(0.0);
  led.fadeUp(300, ogBrightness);

  led.setBrightness(ogBrightness);
}

void stop_recording(LED &led) {
  float ogBrightness = led.getBrightness();

  led.colourFade(280, RED);
  led.fadeDown(200);

  led.setBrightness(ogBrightness);
  led.off();
}

void resume_recording(LED &led) {
  led.colourFade(280, GREEN);
}

void pause_recording(LED &led) {
  led.colourFade(280, YELLOW);
}

void reminding(LED &led) {
  float ogBrightness = led.getBrightness();

  led.set(WHITE);
  led.setBrightness(1.0);
  led.blink(500, 2);

  led.setBrightness(ogBrightness);
}

void thinking(LED &led) {
  led.set(WHITE);
  led.blink(600, 1, true);
}

void confirm(LED &led) {
  float ogBrightness = led.getBrightness();

  led.set(GREEN);
  led.setBrightness(0.0);
  led.fadeUp(100, ogBrightness);
  delay(100);
  led.fadeDown(300);
  led.setBrightness(ogBrightness);
}

void sleep(LED &led) {
  float ogBrightness = led.getBrightness();

  led.fadeUp(400);
  led.colourFade(280, ORANGE);
  led.fadeDown(800,0.7);
  led.fadeDown(200);

  led.setBrightness(ogBrightness);
  led.off();
}

#endif