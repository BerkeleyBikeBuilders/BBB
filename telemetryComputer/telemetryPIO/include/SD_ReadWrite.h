#ifndef SD_READ_WRITE_H
#define SD_READ_WRITE_H

#include <FS.h>
#include <SD.h>
#include <SPI.h>

// SPI interface and file handling variables
SPIClass spi(VSPI);                              // Uses the VSPI interface for SD cards
constexpr int CHAR_ARRAY_LEN = 25;               // Maximum file path length
char filename[CHAR_ARRAY_LEN] = {0};             // Character array for storing the file name
String stringName = "";                          // String variable for the file name
File dataFile;                                   // File object for operations on the SD card
int fileCount = 0;                               // Counter for the number of files in the directory

/**
 * @brief Mounts the SD card and initializes SPI communication.
 *
 * @param SCK The Serial Clock (SCK) pin number.
 * @param MISO The Master In Slave Out (MISO) pin number.
 * @param MOSI The Master Out Slave In (MOSI) pin number.
 * @param CS The Chip Select (CS) pin number.
 * @return true if the SD card is successfully mounted, false otherwise.
 */
bool sdMount(int SCK, int MISO, int MOSI, int CS) {
    spi.begin(SCK, MISO, MOSI, CS);

    if (!SD.begin(CS, spi, 80000000)) {          // Initialize SD card with an 80 MHz clock speed
        Serial.println("ERROR: Failed to mount card. Ensure it is formatted as MS-DOS (FAT32).");
        return false;
    }
    Serial.println("SD card mounted successfully.");
    return true;
}

/**
 * @brief Counts the number of files in the "Telemetry" directory.
 *
 * @return The number of files in the "Telemetry" directory, or -1 if an error occurs.
 */
int countFiles() {
    if (!SD.exists("/Telemetry")) {
        // Create the directory if it doesn't exist
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

    while (File entry = dir.openNextFile()) {    // Iterate through files in the directory
        tally++;
        entry.close();
    }
    return tally;
}

/**
 * @brief Creates a new file in the "Telemetry" directory with a unique name.
 *
 * @return true if the file is created successfully, false otherwise.
 */
bool createFile() {
    fileCount = countFiles();
    if (fileCount < 0) {
        Serial.println("ERROR: Unable to create file due to countFiles error.");
        return false;
    }

    stringName = "/Telemetry/data" + String(fileCount) + ".csv";
    stringName.toCharArray(filename, CHAR_ARRAY_LEN);

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
 * @brief Appends a message to the most recently created file.
 *
 * @param message The line to append to the file.
 * @return true if the message is appended successfully, false otherwise.
 */
bool appendFile(const String& message) {
    File file = SD.open(filename, FILE_APPEND);
    if (!file) {
        Serial.println("ERROR: Cannot open file for appending.");
        return false;
    }

    if (file.print(message)) {
        Serial.println("Message appended successfully.");
        file.close();
        return true;
    } else {
        Serial.println("ERROR: Append operation failed.");
        file.close();
        return false;
    }
}

#endif
