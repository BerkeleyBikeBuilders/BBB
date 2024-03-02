#include "esp32-hal-spi.h"
#include "Arduino.h"
#ifndef SD_CLASS_H
#define SD_CLASS_H


// SETUP
#include "FS.h"
#include "SD.h"
#include "SPI.h"

SPIClass spi = SPIClass(VSPI);

#define charArrayLen 25 // raise this if the file path name grows
char filename[charArrayLen] = {0}; // character array variable to store our converted date string for SD.open()
String stringName = "";
String message;
File dataFile;
File dir;
int fileCount = -1; // to keep track of the number of files
// SETUP


bool SD_mount(int SCK, int MISO, int MOSI, int CS) {
  spi.begin(SCK, MISO, MOSI, CS);
  // spi.setDataMode(SPI_MODE0);
  // spi.begin(SCK, MISO, MOSI, CS);

  if (!SD.begin(CS, spi, 80000000)) { //4000000
    Serial.println("Card Mount Failed");
    return false;
  } else {
    Serial.println("Card Mounted");
    return true;
  }
}


int countFiles() {
  int tally = 0;
  // Open "Telemetry" directory
  if (!SD.exists("/Telemetry")) {
    SD.mkdir("/Telemetry");
  }

  File dir = SD.open("/Telemetry");
  File entry;

  while (true) {
    entry = dir.openNextFile();
    if (!entry) {
      return tally;
    }
    tally += 1;
    entry.close();
  }
}

void createFile(int fileCount) {
  fileCount = countFiles();

  if (!SD.exists("/Telemetry")) {
    SD.mkdir("/Telemetry");
  }

  stringName = String("/Telemetry/data" + String(fileCount / 10) + String(fileCount % 10) + ".csv"); // the '/' is floor division for integers

  stringName.toCharArray(filename, charArrayLen);
  dataFile = SD.open(filename, FILE_WRITE, true);
  delay(10);
  Serial.print("New File: ");
  Serial.println(stringName);
  dataFile.close();
}

void appendFile(String message) {
  /**
    DESCRIPTION:
    appends more text to the opened file.

    PARAMETERS:
    'message': the line you want to insert.
  */

  //message = String(dateTime) + "," + String(shockPosition) + "," + String(forkPosition) + "\n";
  //Serial.printf("Appending to: %s\n", dataFile);

  File file = SD.open(filename, FILE_APPEND);
  if (!file) {
    Serial.println("Can't open file for appending");
    return;
  }
  if (file.print(message)) {
    //Serial.println("Appended");
  } else {
    //Serial.println("Append failed");
  }
  file.close();
}

#endif