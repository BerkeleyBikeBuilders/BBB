//BATTERY
#include "Battery.h"
const int Battery_pin = 16;
const float compensator = battery_factor(); //the adjust for the internal impedance of the ESP board 

//BUTTONS
#include "Buttons.h"
const int Butt_pin = 33;

//LED
#include "LED.h"
#include "LED_Behaviors.h"
const int RPIN = 17;
const int GPIN = 16;
const int BPIN = 4;
LED led;

//LINEAR POTENTIOMETER
#include "Potentiometer.h"
LPOT fork_meter;
const int fork_pin = 15;
const float fork_length = 100;
float forkPosition;

LPOT shock_meter;
const int shock_pin = 2;
const float shock_length = 200;
float shockPosition;

//SD CARD
#include "SD_ReadWrite.h"
#define SCK 18
#define MISO 19
#define MOSI 23
#define CS 5
// int dateTime = 1;

String start_message = "Time,Shock Position,Fork Position"; //the "\n" is added via the function so new columns can be easily added.
String stop_message = ""; //left empty for now
String resume_message = "";
String pause_message = "Paused, Paused, Paused";
String recording_message;


void setup() {
  // Initialize Serial communication at a baud rate of 115200
  Serial.begin(9600);
  
  //LED
  led.create(RPIN, GPIN, BPIN);
  led.l = 0.05;
  //reminding(led);

  //BATTERY
  pinMode(Battery_pin, INPUT);
  float voltage = vibe_check(compensator, Battery_pin);
  battery_display(led, voltage); //flashes the battery signal when turned on.

  //SD CARD
  SD_mount(SCK, MISO, MOSI, CS);

  //BUTTON
  pinMode(Butt_pin, INPUT);

  //LINEAR POTENTIOMETER
  fork_meter.create(fork_pin, fork_length);
  shock_meter.create(shock_pin, shock_length);
}

void loop() {
  // Add a delay to prevent too much output (optional)
  delay(10);

  buttonReading(Butt_pin, led, start_message, stop_message, resume_message, pause_message);

  if (state == "start" || state == "resume"){
   shockPosition = shock_meter.read();
   forkPosition = fork_meter.read();
   recording_message = String(millis() / 1000.0) + "," + String(shockPosition) + "," + String(forkPosition);

   appendFile(recording_message + "\n");

   // dateTime += 1;
  }
}
