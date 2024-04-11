// BATTERY
#include "Battery.h"
const int batteryPin = 16;
const float compensator = batteryFactor(); //this compensates for the voltage error from the ESP board's internal impedance

// BUTTONS
#include "Buttons.h"
const int buttonPin = 32;

// LED
#include "LED.h"
#include "LED_Behaviors.h"
const int RPIN = 27;
const int GPIN = 26;
const int BPIN = 25;
LED led; //initiates the LED object (documented in LED.h)

// LINEAR POTENTIOMETER
#include "Potentiometer.h"
LPOT forkMeter;
const int forkPin = 35;
const float forkLength = 200;
float forkPosition;
//LPOT shock_meter;
//const int shock_pin = 17;
//const float shock_length = 200;
//float shockPosition;

// SD CARD
#include "SD_ReadWrite.h"
#define MISO 18
#define SCK 23
#define MOSI 19
#define CS 22
//int dateTime = 1;

String startMessage = "Time (secs),Fork Position"; // the "\n" is added via the function so new columns can be easily added.
String stopMessage = ""; // left empty for now
String resumeMessage = "";
String pauseMessage = "Paused, Paused";
String recordingMessage;


void setup() {
  // Initialize Serial communication at a baud rate of 115200
  Serial.begin(9600);
  
  // LED
  led.create(RPIN, GPIN, BPIN);

  // BATTERY
  pinMode(batteryPin, INPUT);
  float voltage = readVoltage(compensator, batteryPin);
  displayBattery(led, voltage); // shows battery status when turned on.

  // SD CARD
  while (!SD_mount(SCK, MISO, MOSI, CS)) {
    thinking(led);
  }

  // BUTTON
  pinMode(buttonPin, INPUT);
  customise_buttonReading(startMessage, stopMessage, resumeMessage, pauseMessage);

  // LINEAR POTENTIOMETER
  forkMeter.create(forkPin, forkLength);
  //shock_meter.create(shock_pin, shock_length);
}

void loop() {
  // Add a delay to prevent too much output (optional)
  delay(10);
  buttonReading(buttonPin, led);
 

  if (is_recording()) {
   forkPosition = forkMeter.read();
    Serial.println(forkPosition);
   recordingMessage = String(millis() / 1000.0) + "," + String(forkPosition); // you can append new columns here

   appendFile(recordingMessage + "\n");
  } else {
    // while the device isn't doing anything memory-intensive: idk maybe check the battery level?
  }
}
