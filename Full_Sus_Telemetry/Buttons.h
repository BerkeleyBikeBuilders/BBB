#ifndef BUTTONS_H
#define BUTTONS_H

// Dependencies:
#include "LED_Behaviors.h"
#include "SD_ReadWrite.h"

// SETUP
String state = "stop"; //'stop' 'start' 'pause' 'resume' 
bool timerRunning = false;
const int button_threshold = 3500;
const int hold_t = 700;

long endTime;
long startTime;
long timeGap;
// SETUP


void pauseFunction(LED &led, String resume_message,  String pause_message) {
    /**
    DESCRIPTION:
    pause or resume the recording.

    PARAMETERS:
    'led': the target LED instance.
    'resume_message': the row to append when resumed (THE "\n" IS AUTOMATICALLY INCLUDED!)
    'pause_message': the row to append when paused (SAME THING!)
    */
    
    if (state == "pause"){
        Serial.println("resume");

        if (resume_message != ""){
          appendFile(resume_message + "\n");
        }

        state = "resume";
        resume_recording(led);
        return;
    }
    
    if (state == "resume" || state == "start"){
        Serial.println("pause");

        if (pause_message != ""){
          appendFile(pause_message + "\n");
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

    if (state == "stop"){
        Serial.println("start");
        
        state = "start";
        start_recording(led);
        createFile(fileCount);

        if (start_message != ""){
          appendFile(start_message + "\n");
        }
        return;
    }

    if (state == "start" || state == "resume" || state == "pause"){
        Serial.println("finished recording");
        if (stop_message != ""){
          appendFile(stop_message + "\n");
        }

        state = "stop";
        stop_recording(led);
        return;
    }
}

void buttonReading(const int Butt_pin, LED &led, String start_message, String stop_message, String resume_message,  String pause_message){
    /**
    DESCRIPTION:
    checks if the button's pressed and its duration.

    PARAMETERS:
    'Butt_pin': the input pin connected to the button.
    'led': the target LED instance.
    
    'start_message': the row to append when started. 
    'stop_message': the row to append when stopped.
    'resume_message': the row to append when resumed.
    'pause_message': the row to append when paused.
        NOTE: THE "\n" IS AUTOMATICALLY INCLUDED!
    */

    int butVal = analogRead(Butt_pin); //record button value

    if (butVal >= button_threshold && !timerRunning) {
        startTime = millis();
        timerRunning = true;
    }

    endTime = millis();
    timeGap = endTime - startTime;

    if (timeGap >= hold_t && timerRunning){      
            longHoldFunction(led, start_message, stop_message);
     
            //waits while the button is still held down.
            while (butVal >= button_threshold){
                butVal = analogRead(Butt_pin);
            }
        }
    
    else if (butVal < button_threshold && timerRunning && state != "stop"){
        pauseFunction(led, resume_message, pause_message);
    } 

    if (butVal < button_threshold){
      timerRunning = false;
    }
}

#endif