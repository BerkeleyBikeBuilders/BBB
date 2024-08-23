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
         * @brief Initialize the Button with the specified pins.
         *
         * @param redPIN Red pin for the LED.
         * @param greenPIN Green pin for the LED.
         * @param bluePIN Blue pin for the LED.
         */
        void create(int buttonPinout, LED &signalLED) {
            buttonPin= buttonPinout;
            led = signalLED;

            pinMode(buttonPinout, INPUT);
        }

        /**
         * @brief Pauses or resumes the recording.
         * @param led The target LED instance.
         * @param resumeMsg The message to append when resumed (newline is automatically included).
         * @param pauseMsg The message to append when paused (newline is automatically included).
         */
        void pauseFunction(String resumeMsg, String pauseMsg) {
            if (state == PAUSE) {
                // Serial.println("resume");

                if (!resumeMsg.isEmpty()) {
                    appendFile(resumeMsg + "\n");
                }

                state = RESUME;
                resume_recording(led);
                return;
            }

            if (state == RESUME || state == START) {
                Serial.println("pause");

                if (!pauseMsg.isEmpty()) {
                    appendFile(pauseMsg + "\n");
                }

                state = PAUSE;
                pause_recording(led);
                return;
            }
        }

        /**
         * @brief Pauses or resumes the recording.
         * @param led The target LED instance.
         * @param resumeMsg The message to append when resumed (newline is automatically included).
         * @param pauseMsg The message to append when paused (newline is automatically included).
         */
        void pauseFunction() {
            if (state == PAUSE) {
                if (!resumeMsg.isEmpty()) {
                    appendFile(resumeMsg + "\n");
                }

                state = RESUME;
                resume_recording(led);
            } else if (state == RESUME || state == START) {
                if (!pauseMsg.isEmpty()) {
                    appendFile(pauseMsg + "\n");
                }

                state = PAUSE;
                pause_recording(led);
            }
        }

        /**
         * @brief Starts or stops the recording.
         * @param led The target LED instance.
         * @param startMsg The message to append when started (newline is automatically included).
         * @param stopMsg The message to append when stopped (newline is automatically included).
         */
        void longHoldFunction() {
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
                stop_recording(led);
            }
        }

        /**
         * @brief Re-configures the custom messages for the different button presses.
         * @param startMsg The message to append when started.
         * @param stopMsg The message to append when stopped.
         * @param resumeMsg The message to append when resumed.
         * @param pauseMsg The message to append when paused.
         */
        void customiseButtonMessage(const String& startMessage, const String& stopMessage, const String& resumeMessage, const String& pauseMessage) {
            startMsg = startMessage;
            stopMsg = stopMessage;
            resumeMsg = resumeMessage;
            pauseMsg = pauseMessage;
        }

        /**
         * @brief Checks if the button is pressed and its duration, adds messages to the data-collection table, and returns the updated recording state.
         * @param buttonPin The input pin connected to the button.
         * @param led The target LED instance.
         * @return The current recording state as a string.
         */
        String buttonReading() {
            int buttonValue = analogRead(buttonPin); // Record button value.

            if (buttonValue >= buttonThreshold && !timerRunning) {
                startTime = millis();
                timerRunning = true;
            }

            endTime = millis();
            timeGap = endTime - startTime;

            if (timeGap >= holdTime && timerRunning) {
                longHoldFunction();

                // waits while the button is still held down.
                while (buttonValue >= buttonThreshold) {
                    buttonValue = analogRead(buttonPin);
                    delay(10);  // Avoid tight loops
                }
            }

            else if (buttonValue < buttonThreshold && timerRunning && state != STOP) {
                pauseFunction();
            }

            if (buttonValue < buttonThreshold) {
                timerRunning = false;
            }

            // Convert enum state to string for return
            switch (state) {
                case STOP: return "stop";
                case START: return "start";
                case PAUSE: return "pause";
                case RESUME: return "resume";
                default: return "unknown";
            }
        }

        /**
         * @brief Returns whether the device is currently recording data.
         * @return True if recording, false otherwise.
         */
        bool isRecording() {
            return (state == START || state == RESUME);
        }
};

#endif