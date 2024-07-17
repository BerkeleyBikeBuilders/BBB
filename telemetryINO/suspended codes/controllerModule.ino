// BATTERY
#include "Battery.h"
const int Battery_pin = 16;
const float compensator = battery_factor();
bool battery_notified = false;

// BUTTON
#include "Buttons.h"
const int Butt_pin = 26;
String status; //stored recording status for syncing via ESP-NOW

// LED
#include "LED.h"
#include "LED_Behaviors.h"
const int RPIN = 25;
const int GPIN = 33;
const int BPIN = 32;
LED led;

// LINEAR POTENTIOMETER
#include "Potentiometer.h"
LPOT fork_meter;
const int fork_pin = 15;
const float fork_length = 100;
float forkPosition;

float shockPosition; // to store recieved readings

// SD CARD
#include "SD_ReadWrite.h"
#define SCK 18
#define MISO 19
#define MOSI 23
#define CS 5
//int dateTime = 1;

String start_message = "Time (secs),Fork Position, Shock Position"; // the "\n" is added via the function so new columns can be easily added.
String stop_message = ""; // left empty for now
String resume_message = "";
String pause_message = "Paused, Paused, Paused";
String recording_message;

// ESP-NOW
#include "ESP_now.h"
broadcastAddress = {0x54, 0x43, 0xB2, 0xA9, 0x33, 0x44};// REPLACE WITH THE MAC Address of the responder module 
    // redefine callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  shockPosition = incomingReadings.shockPosition;
}

void setup() {
  // Initialize Serial communication at a baud rate of 9600
  Serial.begin(9600);
  
  // LED
  led.create(RPIN, GPIN, BPIN);
  led.l = 0.05;

  // BATTERY
  pinMode(Battery_pin, INPUT);
  float voltage = vibe_check(compensator, Battery_pin);
  battery_display(led, voltage); // shows battery status when turned on.

  // SD CARD
  sdMount(SCK, MISO, MOSI, CS);

  // BUTTON
  pinMode(Butt_pin, INPUT);
  void buttonReading = custom_buttonReading(start_message, stop_message, resume_message, pause_message);

  // LINEAR POTENTIOMETER
  fork_meter.create(fork_pin, fork_length);

  // ESP-NOW
  initialise_ESP_NOW();
}

void loop() {
  // Add a delay to prevent too much output (optional)
  delay(10);
  status = buttonReading(Butt_pin, led);

  if (is_recording()){
   // recording
   outgoingReadings.status = status;
   SendData();

   forkPosition = fork_meter.read();
   recording_message = String(millis() * 1000) + "," + String(forkPosition) + "," + String(shockPosition); // you can append new columns here
   appendFile(recording_message + "\n");
   //dateTime += 1;

   battery_notified = false;
  }

  else{
    // while the device isn't doing anything memory-intensive:
    float measured_voltage = vibe_check(compensator, Battery_pin);

    if (battery_notified == false){
      battery_display(led, measured_voltage, true);
      status_notified = true; // makes sure the battery notification is only displayed once while not recording.
    }
  }
}
