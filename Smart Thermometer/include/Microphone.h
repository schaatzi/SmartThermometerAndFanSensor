#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <Arduino.h>

// Microphone configuration constants
#define MIC_PIN 7          // Connect MAX4466 to GPIO7
extern const int sampleCount;        // Number of samples per window (~1 second)
extern const int soundThreshold;     // Number of sound events needed to trigger
extern const int baselineOffset;     // MAIN TUNING VARIABLE: Higher = less sensitive

// Global variables
extern int baselineLevel;           // Will be calibrated in setup()
extern int hitCount;
extern int isFanOn;                 // Declare as extern since it's used globally

// Function declarations
void calibrateMicrophone();
void readMicrophoneLevel();

#endif // MICROPHONE_H