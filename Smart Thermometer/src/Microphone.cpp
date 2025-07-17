#include "Microphone.h"

// Microphone configuration
const int sampleCount = 100;        // Number of samples per window (~1 second)
const int soundThreshold = 25;      // Number of sound events needed to trigger
const int baselineOffset = 1500;    // MAIN TUNING VARIABLE: Higher = less sensitive

// Global variables
int baselineLevel = 2048;           // Will be calibrated in setup()
int hitCount = 0;

void calibrateMicrophone() {
  Serial.println("Calibrating microphone baseline... keep environment quiet");
  delay(2000);
  
  long total = 0;
  for (int i = 0; i < 100; i++) {
    total += analogRead(MIC_PIN);
    delay(10);
  }
  baselineLevel = total / 100;
  Serial.print("Baseline level set to: ");
  Serial.println(baselineLevel);
  Serial.println("Now monitoring for fan noise...");
}

void readMicrophoneLevel() {
  isFanOn = 0;  // Explicitly reset before reading
  hitCount = 0;
  
  // Take samples over ~1 second
  for (int i = 0; i < sampleCount; i++) {
    int micValue = analogRead(MIC_PIN);
    
    // Check for deviation from baseline (sound activity)
    int deviation = abs(micValue - baselineLevel);
    if (deviation > baselineOffset) {
      hitCount++;
    }
    delay(10);  // 100Hz sampling
  }
  
  // Set fan state based on sound threshold
  isFanOn = (hitCount > soundThreshold) ? 1 : 0;
  
  Serial.print("Sound Events: ");
  Serial.print(hitCount);
  Serial.print(" | Fan Status: ");
  Serial.println(isFanOn ? "ON" : "OFF");
}