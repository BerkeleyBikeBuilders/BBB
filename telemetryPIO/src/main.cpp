#include <Arduino.h>
#include <driver/rtc_io.h>


// POWER SUPPLY
#include "Battery.h"
const int batteryPin = 15;
const float compensator = batteryFactor(); // this compensates for the voltage error from the ESP board's internal impedance

// USER INTERFACE
#include "Buttons.h"
#include "LED.h"
#include "LED_Behaviors.h"
LED led; // initiates the LED object

typedef enum {
    BUTTON = GPIO_NUM_27,
    RPIN   = 4,
    GPIN   = 2,
    BPIN   = 13,
} UIpins;

String startMessage  = "Time (secs),Fork Position"; // the "\n" is added via the function so new columns can be easily added.
String stopMessage   = "";                           // left empty for now
String resumeMessage = "";
String pauseMessage  = "Paused, Paused";
String recordingMessage; // ¯\_(ツ)_/¯

// Going to Sleep:
int sleepCounter = 0;

// INPUT SOCKETS
typedef enum {
    PIN1 = 25,
    PIN2 = 26,
    PIN3 = 33,
    PIN4 = 32,
    PIN5 = 35,
    PIN6 = 34,
} InputSockets;

// SD CARD
#include "SD_ReadWrite.h"
typedef enum {
    MISO = 19,
    SCK  = 18,
    MOSI = 23,
    CS   = 5,
} SDmodule;

void setup() {
  Serial.begin(9600); // starts Serial communication

  // POWER SUPPLY
  pinMode(batteryPin, INPUT);
  float voltage = readVoltage(compensator, batteryPin);
  displayBattery(led, voltage); // shows battery status when turned on.

  // USER INTERFACE
  led.create(RPIN, GPIN, BPIN);
  //led.calibrateBrightness(0.3, 0. 5, 0.5);
  pinMode(BUTTON, INPUT);
  customiseButtonMessage(startMessage, stopMessage, resumeMessage, pauseMessage);

  // Going to Sleep:
  rtc_gpio_deinit((gpio_num_t) BUTTON); // revert the rtc_GPIO wake up pin back to a normal digital GPIO.
  //sry I used casting... (◡︵◡')

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
  buttonReading(BUTTON, led);
  Serial.println();

  if (isRecording()) {
    // *code to retrieve sensor data...*

    recordingMessage = String(millis() / 1000.0) + "," + String("data entry"); // you can append new columns here.

    appendFile(recordingMessage + "\n");
  } else {
    sleepCounter++;
    if (sleepCounter > 60000) {
      // about 60000 * 10 miliseconds = 10 minutes.
      Serial.println("I'm going to sleep (=__=)");
      delay(2000);
      esp_sleep_enable_ext0_wakeup((gpio_num_t) BUTTON, HIGH); // assigning the button as the wake up pin. sry I used casting again.
      esp_deep_sleep_start();
    }
  }

  //   if (digitalRead(sdStatus) == HIGH)
  //   {
  //     while (!sdMount(SCK, MISO, MOSI, CS))
  //     {
  //       thinking(led);
  //     }
  //     confirm(led);
  //   }
  // }
}
