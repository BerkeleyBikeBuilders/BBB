/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @file main.cpp                                                                  *
 * @brief Code for the Telemetry Computer for the Bike Builders of Berkeley.       *
 *                                                                                 *
 * Hello! I hope you'll have as much fun working on this as I did!                 *
 *                                                                                 *
 * Started in Fall 2023, the Telemetry Computer will be transformed into a         *
 * general, modular test platform to inform the new mountain bike design           *
 * project in Fall 2024!                                                           *
 *                                                                                 *
 * Bike Builders of Berkeley is a student club at UC Berkeley.                     *
 *                                                                                 *
 * @author Kevin Ying, Ziven Posner                                                *
 * @date 22 August, 2024                                                           *
 *                                                                                 * 
 * @details It relies on the Arduino core libraries and these custom header        * 
 *          files:                                                                 *
 *              Buttons.h                                                          *
 *              LED.h                                                              *
 *              LED_Behaviors.h                                                    *
 *              Battery.h                                                          *
 *              SD_ReadWrite.h                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#include <driver/rtc_io.h>

// USER INTERFACE
#include "../telemetryPIO/include/Buttons.h"
#include "../telemetryPIO/include/LED.h"
#include "../telemetryPIO/include/LED_Behaviors.h"
LED led; // Declares a global LED instance
Button button; // Declares a global Button instance

enum UserInterfacePins {
  BUTTONPIN = 27,
  RPIN = 4,
  GPIN = 2,
  BPIN = 13,
};

// POWER SUPPLY
#include "../telemetryPIO/include/Battery.h"
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
#include "../telemetryPIO/include/SD_ReadWrite.h"
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
  led.calibrateBrightness(0.05F);
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
    showThinking(led);
  } 
  confirmAction(led);
}

void loop() {
  // Add a delay to prevent too much output (optional)
  delay(10);
  Serial.println(button.buttonReading());


  
  if (button.isRecording()) {
    //Serial.println("Recording!");

    sleepCounter = 0;

    // *code to retrieve sensor data...*
    recordingMessage = String(millis() / 1000.0) + "," + String("data entry"); // You can append new columns here (modify 'pauseMessage' line 21 if you do).

    appendFile(recordingMessage + "\n");
  } else {
    //Serial.println("Not recording.");

    sleepCounter++;
    if (sleepCounter > 6000) { // 6000 * ~10 miliseconds ≈ 1 minute.
      Serial.println("I'm going to sleep (=_=)");
      sleep(led); // plays sleep animation
      delay(100);

      esp_sleep_enable_ext0_wakeup((gpio_num_t) BUTTONPIN, HIGH); // Assigning the button as the wake up pin. sry again for using casting.
      esp_deep_sleep_start();
    }
  }
}
