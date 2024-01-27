#ifndef SD_ReadWrite_H
#define SD_ReadWrite_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"

SPIClass spi = SPIClass(VSPI);

char filename[13] = {0}; // character array variable to store our converted date string for SD.open()
String stringName = "";
String message;
File dataFile;
File dir;

int fileCount = 0; // for counting files
//SETUP


void SD_mount(int SCK, int MISO, int MOSI, int CS) {
  spi.begin(SCK, MISO, MOSI, CS);

  if (!SD.begin(CS, spi, 80000000)) {
    Serial.println("Card Mount Failed");
    delay(100);
    return;
  } else {
    Serial.println("Card Mounted");
  }
}

void countFiles() {
  
  fileCount = 0;
  dir = SD.open("/");
  Serial.println("Current Files");
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      Serial.print(F("file Count: "));
      Serial.println(fileCount);
      break;
    }
    // for each file count it
    fileCount++;
    Serial.println(entry.name());
    entry.close();
  }
  //dir.close();
}

void createFile(int fileCount) {
  countFiles();
  if (fileCount+1 > 9){
    stringName = String("/data_" + String(fileCount+1) + ".csv");
  } else {
    stringName = String("/data_0" + String(fileCount+1) + ".csv");
  }
  
  stringName.toCharArray(filename, 13);
  dataFile = SD.open(filename, FILE_WRITE);
  delay(10);
  Serial.print("New File: ");
  Serial.println(stringName);
  dataFile.close();
}

void appendFile(String message) {

  //message = String(dateTime) + "," + String(shockPosition) + "," + String(forkPosition) + "\n";
  //Serial.printf("Appending to: %s\n", dataFile);

  File file = SD.open(filename, FILE_APPEND);
  if (!file) {
    Serial.println("Can't open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

#endif