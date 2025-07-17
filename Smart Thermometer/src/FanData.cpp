#include "FanData.h"
#include <EEPROM.h>
//#include "Globals.h"


int fanStateHistory[FAN_HISTORY_SIZE] = {0};
int fanHistoryIndex = 0;
int startAddressForEEPROM = 1; // Start after your single variable at slot 0 on EEPROM


// Call this function whenever a new fan state is read
void updateFanStateHistory(int newState) {
  fanStateHistory[fanHistoryIndex] = newState;
  fanHistoryIndex = (fanHistoryIndex + 1) % FAN_HISTORY_SIZE;
  writeFanHistoryIndexToEEPROM(); // Save the updated index to EEPROM
}


int totalOfMostRecentFanStates() {
  int count = 0;
  for (int i = 0; i < FAN_HISTORY_SIZE; i++) {
    if (fanStateHistory[i] == 1) {
      count++;
    }
  }
  return count;
}


bool isFanRunningTooMuch(int triggerCount) {
  int count = totalOfMostRecentFanStates();
  if (count >= triggerCount) { // Adjust the threshold as needed
    return true;
  } else {
    return false;
  }
}


void writeFanStateHistoryToEEPROM() {
    for (int i = 0; i < FAN_HISTORY_SIZE; i++) {
    EEPROM.write(startAddressForEEPROM + i, fanStateHistory[i]); // Start after your single variable at slot 0 on EEPROM
    }
    EEPROM.commit();
}

void readFanStateHistoryFromEEPROM() { 
  for (int i = 0; i < FAN_HISTORY_SIZE; i++) {
    fanStateHistory[i] = EEPROM.read(startAddressForEEPROM + i); // Start after your single variable at slot 0 on EEPROM
  }
}



void writeFanHistoryIndexToEEPROM() {
    EEPROM.write(startAddressForEEPROM + FAN_HISTORY_SIZE, fanHistoryIndex);
    EEPROM.commit();
}

int readFanHistoryIndexFromEEPROM() {
    int idx = EEPROM.read(startAddressForEEPROM + FAN_HISTORY_SIZE);
    if (idx == 255 || idx < 0 || idx >= FAN_HISTORY_SIZE) {
        idx = 0; // Reset to 0 if the index is out of bounds or uninitialized
    }
    return idx;
}

void clearFanHistory() {
    for (int i = 0; i < FAN_HISTORY_SIZE; i++) {
        fanStateHistory[i] = 0; // Reset all states to 0
    }
    fanHistoryIndex = 0; // Reset index
    writeFanStateHistoryToEEPROM(); // Save the cleared history to EEPROM
    writeFanHistoryIndexToEEPROM(); // Save the reset index to EEPROM
}