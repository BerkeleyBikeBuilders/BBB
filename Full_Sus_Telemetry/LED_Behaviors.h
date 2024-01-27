#ifndef LED_BEHAVIOURS_H
#define LED_BEHAVIOURS_H

// Dependencies:
#include "LED.h"
#include "Buttons.h"


void start_recording(LED &led) {
  float og_bright = led.l;

  led.set('g');
  led.set_brightness(0.0);
  led.fade_up(300, og_bright);

  led.set_brightness(og_bright);
}

void stop_recording(LED &led) {
  float og_bright = led.l;

  led.colour_fade(280, 'r');
  led.fade_down(200);
  
  led.set_brightness(og_bright);
  led.OFF();
}

void resume_recording(LED &led) {
  led.colour_fade(280, 'g');
}

void pause_recording(LED &led) {
  led.colour_fade(280, 'y');
}

void reminding(LED &led) {
  float og_bright = led.l;

  led.set('w');
  led.l = 1.0;
  led.blink(500, 2);

  led.set_brightness(og_bright);
}

void thinking(LED &led) {
  float og_bright = led.l;
  led.fade_down(200);
  led.set_brightness(og_bright);

  led.set('w');
  led.blink(800, 1, true);
}

void error(LED &led) {
  led.set('r');
  led.blink(800, 2);
}

#endif