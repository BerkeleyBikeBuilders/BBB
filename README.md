# Bike Builders of Berkeley Software (and Hardware) Things

## Description
Hello! This is a directory for code and EE-related hardware created for projects at the Bike Builders of Berkeley. The current projects are:

 - ESP32 Telemetry Computer Fall 2023~

## Project Overviews

**ESP32 Telemetry Computer:**

This project was written on the Platform IO IDE in VSCode, But, you can also access the Arduino IDE version in the telemetryINO folder (be warned its contents might be out of date).
All the .h header files are stored in ./telemetryPIO/include




## Installation
1. Clone the repository:
   
    Go to the folder you want to clone to, and:
    ```bash
    git clone https://github.com/pizzamouse/BBB-Software-Things.git


3. Install IDE:
   
    **Platform IO on VS Code**
   
    Install Platform IO extension on VSCode, or the Arduino IDE online.

    **Arduino IDE**
   
    Install IDE from here: https://www.arduino.cc/en/software
   
    Add ESP32 boards this way: https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/
   
    If you prefer to use the Arduino IDE, to access the project, edit the 'telemetryINO' project, **and remember to first update the code from src/main.cpp**



4. install Libraries:
    I think it's only nessesary for the Arduino IDE.
    - Adafruit BusIO



6. select boards:
    Select either "Adafruit ESP32 Feather" or "WEMOS LOLIN32 Lite"



## Debugging
1. .............A fatal error occurred: No serial data received
   
   Try Changing to upload speed to 115200
   
   Try pressing the reset button, can also help clear processes that's preventing the upload (check your code).

3. 

