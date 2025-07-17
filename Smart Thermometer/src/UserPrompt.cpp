#include <Arduino.h>
#include <EEPROM.h>
#include "UserPrompt.h"
#include "Globals.h"
#include "SequenceToApartmentNumber.h"


void promptUser() {
  Serial.print("Current unit number is ");
  Serial.println(originalUnitNumber);

  if(isInitializerForFanSensor == 1) {
    Serial.println("Enter new unit number to broadcast as a setup broadcast:");
    while (!Serial.available()) {
      delay(10);
    }
      //String input = Serial.readStringUntil('\n');
      //input.trim();
      //int newUnit = input.toInt();
      String broadcastInput = Serial.readStringUntil('\n');
      broadcastInput.trim();
      int broadcastUnit = broadcastInput.toInt();
      sequenceNumber = broadcastUnit;
  } else {
      unsigned long startTime = millis();
      Serial.println("Enter new unit number (you have 5 seconds):");
      while (!Serial.available() && (millis() - startTime < 5000)) {
        delay(10);
      }
      if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        int newUnit = input.toInt();
        originalUnitNumber = newUnit;
        EEPROM.write(0, originalUnitNumber);  // Save to EEPROM
        EEPROM.commit();
        Serial.print("Unit number updated to: ");
        Serial.println(originalUnitNumber);
      } else {
        Serial.println("No input received. Keeping current unit number.");
      }
  }
}

  //unsigned long startTime = millis();
  //while (millis() - startTime < 5000) {
  

void displayApartmentNumber() {
  String apartment = sequenceToApartmentNumber(originalUnitNumber);
  Serial.print("Apartment number for unit ");
  Serial.print(originalUnitNumber);
  Serial.print(" is: ");
  Serial.println(apartment);
}