#ifndef COOLING_H
#define COOLING_H

#include <Arduino.h>

class COOLING {
    private:
        float targetT = 0;
        float measuredT = 0;
        bool on = false;
        unsigned int coolerPin;
        float tempD;

    public:
        void create(float targetTemp, unsigned int coolerPin, float tempDeviation) {
            targetT = targetTemp;
            tempD = tempDeviation;


        }



}





#endif