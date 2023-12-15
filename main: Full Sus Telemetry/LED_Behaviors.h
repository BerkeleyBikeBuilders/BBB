#ifndef LED_BEHAVIOURS_H
#define LED_BEHAVIOURS_H

//SETUP
#include "LED.h"
#include "Buttons.h"
//SETUP


void start_recording(LED &led){
  float og_bright = led.l;

  led.set('g');
  led.set_brightness(0.0);
  led.fade_up(300, og_bright);

  led.set_brightness(og_bright);
}

void stop_recording(LED &led){
  float og_bright = led.l;

  led.colour_fade(280, 'r');
  led.fade_down(200);
  
  led.set_brightness(og_bright);
  led.OFF();
}

void resume_recording(LED &led){
  led.colour_fade(280, 'g');
}

void pause_recording(LED &led){
  led.colour_fade(280, 'y');
}

void reminding(LED &led){
  led.set('b');
  led.blink(660, 2, true);
  led.OFF();
}

#endif