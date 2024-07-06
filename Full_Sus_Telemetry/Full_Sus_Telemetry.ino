// BATTERY
#include "Battery.h"
const int batteryPin = 25;
const float compensator = batteryFactor(); // this compensates for the voltage error from the ESP board's internal impedance

// BUTTONS
#include "Buttons.h"
const int buttonPin = 26;

// LED
#include "LED.h"
#include "LED_Behaviors.h"
const int RPIN = 13;
const int GPIN = 2;
const int BPIN = 15;
LED led; // initiates the LED object (documented in LED.h)

// LINEAR POTENTIOMETER
#include "Potentiometer.h"
LPOT forkMeter;
const int forkPin = 35;
const float forkLength = 200;
float forkPosition;
// LPOT shock_meter;
// const int shock_pin = 17;
// const float shock_length = 200;
// float shockPosition;

// SD CARD
#include "SD_ReadWrite.h"
#define MISO 19
#define SCK 18
#define MOSI 23
#define CS 5
// int dateTime = 1;
//#define sdStatus 1 // use this pin to detect SD card during loop.

String startMessage = "Time (secs),Fork Position"; // the "\n" is added via the function so new columns can be easily added.
String stopMessage = "";                           // left empty for now
String resumeMessage = "";
String pauseMessage = "Paused, Paused";
String recordingMessage;

// MISC
int sleepTimer = 0;

void setup() {
  // Initialize Serial communication at a baud rate of 115200
  Serial.begin(9600);

  // LED
  led.create(RPIN, GPIN, BPIN);
  led.calibrateBrightness(0.3, 0.5, 0.5);

  // BATTERY
  pinMode(batteryPin, INPUT);
  float voltage = readVoltage(compensator, batteryPin);
  displayBattery(led, voltage); // shows battery status when turned on.

  // SD CARD
  while (!sdMount(SCK, MISO, MOSI, CS)) {
    thinking(led);
  } 
  confirm(led);

  //pinMode(sdStatus, INPUT);

  // BUTTON
  pinMode(buttonPin, INPUT);
  customiseButtonReading(startMessage, stopMessage, resumeMessage, pauseMessage);

  // LINEAR POTENTIOMETER
  forkMeter.create(forkPin, forkLength);
  // shock_meter.create(shock_pin, shock_length);
}

void loop() {
  // Add a delay to prevent too much output (optional)
  delay(10);
  buttonReading(buttonPin, led);

  if (isRecording()) {
    forkPosition = forkMeter.read();
    Serial.println(forkPosition);
    recordingMessage = String(millis() / 1000.0) + "," + String(forkPosition); // you can append new columns here

    appendFile(recordingMessage + "\n");
    
    sleepTimer = 0;
  } else {
  //   while the device isn't doing anything memory-intensive:
  // sleepTimer++;
  // if (sleepTimer > 50000) {
  //   esp_sleep_enable_ext0_wakeup(buttonPin, 1); // sleep until button is pressed
  //   ESP.restart();
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
