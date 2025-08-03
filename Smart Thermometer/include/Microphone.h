#ifndef MICROPHONE_H
#define MICROPHONE_H

#include <Arduino.h>
#include "Globals.h"  // BUG FIX: Include Globals.h to avoid duplicate isFanOn declaration

// Microphone configuration constants
#define MIC_PIN 7          // Connect MAX4466 to GPIO7
extern const int sampleCount;        // Number of samples per window (~1 second)
extern const int soundThreshold;     // Number of sound events needed to trigger
extern const int baselineOffset;     // MAIN TUNING VARIABLE: Higher = less sensitive

// Global variables
extern int baselineLevel;           // Will be calibrated in setup()
extern int hitCount;
// BUG FIX: Removed duplicate extern int isFanOn declaration - now using Globals.h

// Function declarations
void calibrateMicrophone();
void readMicrophoneLevel();

#endif // MICROPHONE_H