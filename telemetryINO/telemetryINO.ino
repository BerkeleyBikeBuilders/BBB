// BATTERY
#include "Battery.h"
const int batteryPin = 15;
const float compensator = batteryFactor(); // this compensates for the voltage error from the ESP board's internal impedance

// BUTTONS
#include "Buttons.h"
const int buttonPin = 27;
// gpio_num_t buttonSleepPin = gpio_num_t(buttonPin);
String startMessage = "Time (secs),Fork Position"; // the "\n" is added via the function so new columns can be easily added.
String stopMessage = "";                           // left empty for now
String resumeMessage = "";
String pauseMessage = "Paused, Paused";
String recordingMessage;

// LED
#include "LED.h"
#include "LED_Behaviors.h"
const int RPIN = 13;
const int GPIN = 2;
const int BPIN = 4;
LED led; // initiates the LED object

// LINEAR POTENTIOMETER
// #include "Potentiometer.h"
// LPOT forkMeter;
// const int forkPin = 35;
// const float forkLength = 200;
// float forkPosition;
// LPOT shock_meter;
// const int shock_pin = 17;
// const float shock_length = 200;
// float shockPosition;

// EXPANSION BUS
const int bus1 = 25;
const int bus2 = 26;
const int bus3 = 33;
const int bus4 = 32;
const int bus5 = 35;
const int bus6 = 34;

// SD CARD
#include "SD_ReadWrite.h"
#define MISO 19
#define SCK 18
#define MOSI 23
#define CS 5

// MISC

void setup() {
  // Initialize Serial communication at a baud rate of 115200
  Serial.begin(9600);

  // LED
  led.create(RPIN, GPIN, BPIN);
  //led.calibrateBrightness(0.3, 0. 5, 0.5);

  // BATTERY
  pinMode(batteryPin, INPUT);
  float voltage = readVoltage(compensator, batteryPin);
  displayBattery(led, voltage); // shows battery status when turned on.

  // SD CARD
  while (!sdMount(SCK, MISO, MOSI, CS)) {
    thinking(led);
  } 
  confirm(led);

  // BUTTON
  pinMode(buttonPin, INPUT);
  customiseButtonMessage(startMessage, stopMessage, resumeMessage, pauseMessage);

  // LINEAR POTENTIOMETER
  //forkMeter.create(forkPin, forkLength);
  // shock_meter.create(shock_pin, shock_length);
}

void loop() {
  // Add a delay to prevent too much output (optional)
  delay(10);
  buttonReading(buttonPin, led);
  Serial.println();

  if (isRecording()) {
    //forkPosition = forkMeter.read();
    //Serial.println(forkPosition);
    recordingMessage = String(millis() / 1000.0) + "," + String("data entry"); // you can append new columns here

    appendFile(recordingMessage + "\n");
  } else {
    //while the device isn't doing anything memory-intensive:
    // sleepTimer++;
    // if (sleepTimer > 50000) {
    //   esp_sleep_enable_ext0_wakeup(buttonSleepPin, 1); // sleep until button is pressed
    //   esp_deep_sleep_start();
    // }
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
