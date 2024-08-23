#ifndef SD_ReadWrite_H
#define SD_ReadWrite_H

#include <FS.h>
#include <SD.h>
#include <SPI.h>

// Define SPI interface and file handling variables
SPIClass spi(VSPI);                             // Uses VSPI interface for SD cards
constexpr int charArrayLen = 25;                 // Maximum file path length
char filename[charArrayLen] = {0};               // Character array for file name
String stringName = "";                          // String variable to store file name
String message;                                  // String to hold the message to be written to the file
File dataFile;                                   // File object for writing to the SD card
int fileCount = 0;                               // Variable to keep track of the number of files

/**
 * @brief Mounts the SD card and initializes SPI communication.
 *
 * @param SCK The Serial Clock pin number.
 * @param MISO The Master In Slave Out pin number.
 * @param MOSI The Master Out Slave In pin number.
 * @param CS The Chip Select pin number.
 * @return true if the SD card is successfully mounted, false otherwise.
 */
bool sdMount(int SCK, int MISO, int MOSI, int CS) {
    spi.begin(SCK, MISO, MOSI, CS);

    if (!SD.begin(CS, spi, 80000000)) {           // Initialise SD card at 80 MHz
        Serial.println("ERROR: Failed to mount card, make sure it's formatted as MS-DOS (FAT32).");
        return false;
    }
    Serial.println("Card Mounted");
    return true;
}

/**
 * @brief Counts the number of files in the "Telemetry" directory.
 *
 * @return The number of files in the "Telemetry" directory or -1 if an error occurred.
 */
int countFiles() {
    if (!SD.exists("/Telemetry")) {
        // Create directory if it doesn't exist
        if (!SD.mkdir("/Telemetry")) {
            Serial.println("ERROR: Failed to create /Telemetry directory.");
            return -1;
        }
    }

    int tally = 0;
    File dir = SD.open("/Telemetry");
    if (!dir) {
        Serial.println("ERROR: Failed to open /Telemetry directory.");
        return -1;
    }

    while (File entry = dir.openNextFile()) {     // Count the files in the directory
        tally++;
        entry.close();
    }
    //dir.close();
    return tally;
}

/**
 * @brief Creates a new file in the "Telemetry" directory with a unique name.
 *
 * @return true if the file was created successfully, false otherwise.
 */
bool createFile() {
    fileCount = countFiles();
    if (fileCount < 0) {
        Serial.println("ERROR: Can't create file due to countFiles error.");
        return false;
    }

    stringName = "/Telemetry/data" + String(fileCount) + ".csv";
    stringName.toCharArray(filename, charArrayLen);
    
    dataFile = SD.open(filename, FILE_WRITE);
    if (!dataFile) {
        Serial.println("ERROR: Failed to create new file.");
        return false;
    }

    Serial.print("New file created: ");
    Serial.println(stringName);
    dataFile.close();
    return true;
}

/**
 * @brief Appends a message to the latest created file.
 *
 * @param message The line you want to insert into the file.
 * @return true if the message was appended successfully, false otherwise.
 */
bool appendFile(const String& message) {
    File file = SD.open(filename, FILE_APPEND);
    if (!file) {
    Serial.println("Can't open file for appending");
        return false;
    }
    if (file.print(message)) {
        Serial.println("Appended");
        file.close();
        return true;
    } else {
        Serial.println("Append failed");
        file.close();
        return false;
    }
}

#endif
