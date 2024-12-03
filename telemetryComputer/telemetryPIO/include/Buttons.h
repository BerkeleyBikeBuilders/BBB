#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "LED_Behaviors.h"
#include "SD_ReadWrite.h"

// Enum for recording state
enum RecordingState {
    STOP,
    START,
    PAUSE,
    RESUME
};

class Button {
private:
    RecordingState state = STOP;

    int buttonPin;
    LED led;

    bool timerRunning = false;
    const int buttonThreshold = 3000;
    const int holdTime = 700;

    long startTime = 0;
    long endTime = 0;
    long timeGap = 0;

    // Custom Messages
    String startMsg = "";
    String stopMsg = "";
    String resumeMsg = "";
    String pauseMsg = "";

public:
    /**
     * @brief Initializes the Button with the specified pin and LED.
     *
     * @param buttonPinout The pin connected to the button.
     * @param signalLED The LED instance to control.
     */
    void initialize(int buttonPinout, LED &signalLED) {
        buttonPin = buttonPinout;
        led = signalLED;

        pinMode(buttonPinout, INPUT);
    }

    /**
     * @brief Toggles between pausing and resuming recording.
     *
     * @param resumeMessage Message to append when resumed.
     * @param pauseMessage Message to append when paused.
     */
    void togglePauseResume(const String &resumeMessage, const String &pauseMessage) {
        if (state == PAUSE) {
            if (!resumeMessage.isEmpty()) {
                appendFile(resumeMessage + "\n");
            }
            state = RESUME;
            resumeRecording(led);
        } else if (state == RESUME || state == START) {
            if (!pauseMessage.isEmpty()) {
                appendFile(pauseMessage + "\n");
            }
            state = PAUSE;
            pauseRecording(led);
        }
    }

    /**
     * @brief Toggles between starting and stopping recording.
     */
    void handleLongPress() {
        if (state == STOP) {
            startRecording(led);
            state = START;
            createFile();

            if (!startMsg.isEmpty()) {
                appendFile(startMsg + "\n");
            }
        } else {
            if (!stopMsg.isEmpty()) {
                appendFile(stopMsg + "\n");
            }
            state = STOP;
            stopRecording(led);
        }
    }

    /**
     * @brief Sets custom messages for button actions.
     *
     * @param startMessage Message for starting recording.
     * @param stopMessage Message for stopping recording.
     * @param resumeMessage Message for resuming recording.
     * @param pauseMessage Message for pausing recording.
     */
    void setCustomMessages(const String &startMessage, const String &stopMessage, const String &resumeMessage, const String &pauseMessage) {
        startMsg = startMessage;
        stopMsg = stopMessage;
        resumeMsg = resumeMessage;
        pauseMsg = pauseMessage;
    }

    /**
     * @brief Reads the button state and performs corresponding actions.
     *
     * @return The current recording state as a string.
     */
    String readButtonState() {
        int buttonValue = analogRead(buttonPin);

        if (buttonValue >= buttonThreshold && !timerRunning) {
            startTime = millis();
            timerRunning = true;
        }

        endTime = millis();
        timeGap = endTime - startTime;

        if (timeGap >= holdTime && timerRunning) {
            handleLongPress();

            while (analogRead(buttonPin) >= buttonThreshold) {
                delay(10); // Avoid tight loops
            }
        } else if (buttonValue < buttonThreshold && timerRunning && state != STOP) {
            togglePauseResume(resumeMsg, pauseMsg);
        }

        if (buttonValue < buttonThreshold) {
            timerRunning = false;
        }

        switch (state) {
            case STOP: return "stop";
            case START: return "start";
            case PAUSE: return "pause";
            case RESUME: return "resume";
            default: return "unknown";
        }
    }

    /**
     * @brief Checks if the device is currently recording.
     *
     * @return True if recording, false otherwise.
     */
    bool isRecording() const {
        return (state == START || state == RESUME);
    }
};

#endif
