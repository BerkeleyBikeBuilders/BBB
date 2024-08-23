#include <Arduino.h>
#include <driver/rtc_io.h>

// USER INTERFACE
#include "Buttons.h"
#include "LED.h"
#include "LED_Behaviors.h"
LED led; // Declares a global LED instance
Button button; // Declares a global Button instance

enum UserInterfacePins {
  BUTTONPIN = 27,
  RPIN = 4,
  GPIN = 2,
  BPIN = 13,
};

// POWER SUPPLY
#include "Battery.h"
const int batteryPin = 15;
BatteryVoltageSensor battery;

// Sleep:
int sleepCounter = 0;

// INPUT SOCKETS
enum InputSockets {
  PIN1 = 25,
  PIN2 = 26,
  PIN3 = 33,
  PIN4 = 32,
  PIN5 = 35,
  PIN6 = 34,
};

// SD CARD
#include "SD_ReadWrite.h"
#define MISO 19
#define SCK 18
#define MOSI 23
#define CS 5
String startMessage  = "Time (secs),Fork Position";     // The "\n" are added in the methods, so new columns can be easily added.
String stopMessage;
String resumeMessage;
String pauseMessage  = "Paused, Paused";               // Make sure that the number of commas is the same as the number of columns!
String recordingMessage;

void setup() {
  Serial.begin(9600); // Starts Serial communication.

  // USER INTERFACE
  led.create(RPIN, GPIN, BPIN);
  //led.calibrateBrightness(0.8);
  button.create(BUTTONPIN, led);
  button.customiseButtonMessage(startMessage, stopMessage, resumeMessage, pauseMessage); // The messages will be written into the csv file.

  // POWER SUPPLY
  battery.create(batteryPin);
  battery.displayBattery(led); // Shows battery status when turned on.

  // Sleep:
  rtc_gpio_deinit((gpio_num_t) BUTTONPIN); // Revert the rtc_GPIO wake up pin back to a normal (digital?) GPIO.
  //sry I used casting...

  // INPUT SOCKETS
  pinMode(PIN1, INPUT);
  pinMode(PIN2, INPUT);
  pinMode(PIN3, INPUT);
  pinMode(PIN4, INPUT);
  pinMode(PIN5, INPUT);
  pinMode(PIN6, INPUT);

  // SD CARD
  while (!sdMount(SCK, MISO, MOSI, CS)) {
    thinking(led);
  } 
  confirm(led);
}

void loop() {
  // Add a delay to prevent too much output (optional)
  delay(10);
  button.buttonReading();
  
  if (button.isRecording()) {
    //Serial.println("Recording!");

    // *code to retrieve sensor data...*
    recordingMessage = String(millis() / 1000.0) + "," + String("data entry"); // You can append new columns here (modify 'pauseMessage' line 21 if you do).

    appendFile(recordingMessage + "\n");
  } else {
    //Serial.println("Not recording.");

    idle(led);

    sleepCounter++;
    if (sleepCounter > 18000) { // 18000 * ~10 miliseconds ≈ 3 minutes.
      Serial.println("I'm going to sleep (=_=)");
      sleep(led); // plays sleep animation
      delay(100);

      esp_sleep_enable_ext0_wakeup((gpio_num_t) BUTTONPIN, HIGH); // Assigning the button as the wake up pin. sry again for using casting.
      esp_deep_sleep_start();
    }
  }
}
