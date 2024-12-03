#ifndef LED_H
#define LED_H

#include <Arduino.h>

// Enumeration for predefined colors
typedef enum {
    RED,
    GREEN,
    BLUE,
    ORANGE,
    YELLOW,
    BLACK, // Legacy, use off() or fadeDown instead
    WHITE
} Colour;

class LED {
private:
    int rPin, gPin, bPin; // Pins for red, green, and blue channels
    Colour colour = WHITE; // Default LED color
    int r = 255, g = 255, b = 255; // Default RGB values for white
    float l = 1.0F; // Current brightness (lightness)
    float peakl = 1.0F; // Peak brightness for calibration

    // Helper struct to represent RGB values
    struct RGB {
        int red, green, blue;
    };

    /**
     * @brief Convert a Colour enum to corresponding RGB values.
     * 
     * @param colour Colour to convert (e.g., RED, GREEN).
     * @return RGB struct with red, green, and blue values.
     */
    RGB getColourRGB(Colour colour) {
        switch (colour) {
            case RED:    return {255, 10, 0};
            case GREEN:  return {90, 255, 0};
            case BLUE:   return {0, 47, 255};
            case ORANGE: return {255, 100, 0};
            case YELLOW: return {255, 200, 0};
            case BLACK:  return {0, 0, 0};
            case WHITE:  return {255, 255, 255};
            default:     return {255, 255, 255};
        }
    }

    /**
     * @brief Helper function to cross-fade the LED to target RGB values.
     * 
     * @param time Duration of fade in milliseconds.
     * @param targetRed Target red value.
     * @param targetGreen Target green value.
     * @param targetBlue Target blue value.
     */
    void helperChangeColour(int time, int targetRed, int targetGreen, int targetBlue) {
        int redDiff = abs(targetRed - r);
        int greenDiff = abs(targetGreen - g);
        int blueDiff = abs(targetBlue - b);

        if (redDiff == 0 && greenDiff == 0 && blueDiff == 0) return;

        int dr = redDiff == 0 ? 0 : (targetRed - r) / redDiff;
        int dg = greenDiff == 0 ? 0 : (targetGreen - g) / greenDiff;
        int db = blueDiff == 0 ? 0 : (targetBlue - b) / blueDiff;
        int dt = round(time / max({redDiff, greenDiff, blueDiff})) + 1;

        while (max({redDiff, greenDiff, blueDiff}) > 0) {
            if (redDiff > 0) { r += dr; redDiff--; }
            if (greenDiff > 0) { g += dg; greenDiff--; }
            if (blueDiff > 0) { b += db; blueDiff--; }
            on();
            delay(dt);
        }
    }

public:
    // Initialization
    void create(int redPIN, int greenPIN, int bluePIN) {
        rPin = redPIN;
        gPin = greenPIN;
        bPin = bluePIN;

        pinMode(rPin, OUTPUT);
        pinMode(gPin, OUTPUT);
        pinMode(bPin, OUTPUT);
    }

    // Brightness control
    float getBrightness() const { return l; }
    void calibrateBrightness(float peakBrightness) { peakl = peakBrightness; }
    void setBrightness(float brightness) { l = constrain(brightness, 0.0F, 1.0F); }

    // Color control
    void set(Colour setColour) {
        RGB rgb = getColourRGB(setColour);
        r = rgb.red;
        g = rgb.green;
        b = rgb.blue;
    }
    void set(int red, int green, int blue) { r = red; g = green; b = blue; }

    // Turn LED on/off
    void on() const {
        analogWrite(rPin, constrain(round(r * l * peakl), 0, 255));
        analogWrite(gPin, constrain(round(g * l * peakl), 0, 255));
        analogWrite(bPin, constrain(round(b * l * peakl), 0, 255));
    }
    void off() const {
        analogWrite(rPin, 0);
        analogWrite(gPin, 0);
        analogWrite(bPin, 0);
    }

    // Fading
    void fadeDown(int time, float brightness = 0.0F) {
        brightness = constrain(brightness, 0.0F, 1.0F);
        if (brightness < l) {
            int stepDelay = abs(round(time / 100 / (l - brightness)));
            while (l >= brightness) {
                on();
                l -= 0.01F;
                delay(stepDelay);
            }
            l = brightness;
        }
    }
    void fadeUp(int time, float brightness = 1.0F) {
        brightness = constrain(brightness, 0.0F, 1.0F);
        if (brightness > l) {
            int stepDelay = abs(round(time / 100 / (brightness - l)));
            while (l <= brightness) {
                on();
                l += 0.01F;
                delay(stepDelay);
            }
            l = brightness;
        }
    }

    // Blinking
    void blink(int time = 1000, int num = 3, bool fadeOut = false) {
        int blinkOn = fadeOut ? round(time / num * 0.4F) : round(time / num * 0.15F);
        int blinkOff = fadeOut ? round(time / num * 0.6F) : round(time / num * 0.85F);

        for (int i = 0; i < num; i++) {
            if (fadeOut) {
                float savedBrightness = l;
                fadeDown(blinkOn);
                fadeUp(blinkOn, savedBrightness);
            } else {
                off();
                delay(blinkOff);
                on();
                delay(blinkOn);
            }
        }
    }

    // Cross-fading
    void changeColour(int time, Colour colour) {
        RGB finalColour = getColourRGB(colour);
        helperChangeColour(time, finalColour.red, finalColour.green, finalColour.blue);
    }
    void changeColour(int time, int targetRed, int targetGreen, int targetBlue) {
        helperChangeColour(time, targetRed, targetGreen, targetBlue);
    }
};

#endif
