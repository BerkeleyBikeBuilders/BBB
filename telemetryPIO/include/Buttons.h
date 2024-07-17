#ifndef BUTTONS_H
#define BUTTONS_H

// SETUP
#include "LED_Behaviors.h"
#include "SD_ReadWrite.h"
// SETUP

String state = "stop"; // "stop" "start" "pause" "resume"
bool timerRunning = false;
const int button_threshold = 3500;
const int hold_t = 700;

long endTime;
long startTime;
long timeGap;

String start_m;
String stop_m;
String resume_m;
String pause_m;
// SETUP

void pauseFunction(LED &led, String resumeMessage, String pauseMessage) {
    /**
    DESCRIPTION:
    pause or resume the recording.

    PARAMETERS:
    'led': the target LED instance.
    'resumeMessage': the row to append when resumed (THE "\n" IS AUTOMATICALLY INCLUDED!)
    'pauseMessage': the row to append when paused (SAME THING!)
    */

    if (state == "pause") {
        // Serial.println("resume");

        if (resumeMessage != "") {
            appendFile(resumeMessage + "\n");
        }

        state = "resume";
        resume_recording(led);
        return;
    }

    if (state == "resume" || state == "start") {
        Serial.println("pause");

        if (pauseMessage != "") {
            appendFile(pauseMessage + "\n");
        }

        state = "pause";
        pause_recording(led);
        return;
    }
}

void longHoldFunction(LED &led, String start_message, String stop_message) {
    /**
    DESCRIPTION:
    starts or stops the recording.

    PARAMETERS:
    'led': the target LED instance.
    'start_message': the row to append when started (THE "\n" IS AUTOMATICALLY INCLUDED!)
    'stop_message': the row to append when stopped (SAME THING!)
    */

    if (state == "stop") {
        /// Serial.println("start");

        state = "start";
        startRecording(led);
        createFile(fileCount);

        if (start_message != "") {
            appendFile(start_message + "\n");
        }
        return;
    }

    if (state == "start" || state == "resume" || state == "pause") {
        // Serial.println("stop");
        if (stop_message != "") {
            appendFile(stop_message + "\n");
        }

        state = "stop";
        stop_recording(led);
        return;
    }
}

void customiseButtonMessage(String START_M, String STOP_M, String RESUME_M, String PAUSE_M) {
    /**
    DESCRIPTION:
    re-configure the custom messages for the different
    button presses.

    PARAMETERS:
    'Butt_pin': the input pin connected to the button.
    'led': the target LED instance.

    'start_message': the row to append when started.
    'stop_message': the row to append when stopped.
    'resumeMessage': the row to append when resumed.
    'pauseMessage': the row to append when paused.
        NOTE: THE "\n" WILL BE AUTOMATICALLY INCLUDED!
    */

    start_m = START_M;
    stop_m = STOP_M;
    resume_m = RESUME_M;
    pause_m = PAUSE_M;
}

String buttonReading(const int Butt_pin, LED &led) {
    /**
    DESCRIPTION:
    1: checks if the button's pressed and its duration,
    2: add messages to the data-collection table,
    3: returns the updated recording state.

    PARAMETERS:
    'Butt_pin': the input pin connected to the button.
    'led': the target LED instance.
    */

    int butVal = analogRead(Butt_pin); // record button value

    if (butVal >= button_threshold && !timerRunning) {
        startTime = millis();
        timerRunning = true;
    }

    endTime = millis();
    timeGap = endTime - startTime;

    if (timeGap >= hold_t && timerRunning) {
        longHoldFunction(led, start_m, stop_m);

        // waits while the button is still held down.
        while (butVal >= button_threshold) {
            butVal = analogRead(Butt_pin);
        }
    }

    else if (butVal < button_threshold && timerRunning && state != "stop") {
        pauseFunction(led, resume_m, pause_m);
    }

    if (butVal < button_threshold) {
        timerRunning = false;
    }

    return state;
}

bool isRecording() {
    /**
    DESCRIPTION:
    returns if the device is recording data or not
    */

    if (state == "start" || state == "resume") {
        return true;
    } else {
        return false;
    }
}

#endif