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
void stopRecording(LED &led) {
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
void resumeRecording(LED &led) {
    led.changeColour(280, GREEN);
}

/**
 * @brief Pauses the recording process by changing the LED color to yellow.
 * 
 * @param led The LED instance to control.
 */
void pauseRecording(LED &led) {
    led.changeColour(280, YELLOW);
}

/**
 * @brief Shows a "thinking" animation by blinking the LED on/off in white.
 * 
 * @param led The LED instance to control.
 */
void showThinking(LED &led) {
    led.set(WHITE);
    led.on();
    delay(800);
    led.off();
    delay(200);
}

/**
 * @brief Confirms an action by briefly fading up and then down the LED in green.
 * 
 * @param led The LED instance to control.
 */
void confirmAction(LED &led) {
    float initBrightness = led.getBrightness();

    led.setBrightness(0.0);
    led.set(GREEN);
    led.fadeUp(100, initBrightness);
    delay(100);
    led.fadeDown(300);
    led.off();
    led.setBrightness(initBrightness);
}

/**
 * @brief Shows a "sleep" animation by gradually reducing the LED brightness while blinking.
 * 
 * @param led The LED instance to control.
 */
void showSleepAnimation(LED &led) {
    int initBrightness = led.getBrightness();
    led.changeColour(500, WHITE);

    led.fadeDown(800);
    led.fadeUp(800);
    led.fadeDown(800);
    delay(200);
    led.fadeUp(800);
    led.fadeDown(1000);

    led.off();
    led.setBrightness(initBrightness);
}

#endif
