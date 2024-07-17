// BATTERY
#include "Battery.h"
const int Battery_pin = 16;
const float compensator = battery_factor();
bool battery_notified = false;

// BUTTON

// LED
#include "LED.h"
#include "LED_Behaviors.h"
const int RPIN = 25;
const int GPIN = 33;
const int BPIN = 32;
LED led;

// LINEAR POTENTIOMETER
#include "Potentiometer.h"
LPOT shock_meter;
const int shock_pin = 17;
const float shock_length = 200;
//float shockPosition;

// ESP-NOW
#include "ESP_now.h"
broadcastAddress = {0x54, 0x43, 0xB2, 0xA9, 0x23, 0x28};// REPLACE WITH THE MAC Address of the controller module 
    
    // redefine callback for recieved data
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  status = incomingReadings.status;
}

void setup() {
  // Initialize Serial communication at a baud rate of 9600
  Serial.begin(9600);
  
/**
  // LED
  led.create(RPIN, GPIN, BPIN);
  led.l = 0.05;

  // BATTERY
  pinMode(Battery_pin, INPUT);
  float voltage = vibe_check(compensator, Battery_pin);
  battery_display(led, voltage); // shows battery status when turned on.

  // BUTTON
  
  // LINEAR POTENTIOMETER
  shock_meter.create(shock_pin, shock_length);
*/

  // ESP-NOW
  initialise_ESP_NOW();
}

void loop() {
  // Add a delay to prevent too much output (optional)
  delay(10);

  if (status == "resume" || status == "start"){
   // while recording:
   outgoingReadings.shockPosition = "works!!"; //shock_meter.read();
   SendData();
  }

  else{
    // while the device isn't doing anything memory-intensive;
    // probably could be used to check battery level using the button
    // or wait for recording signal from controller board.
  }
}
