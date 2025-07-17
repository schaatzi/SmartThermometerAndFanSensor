#ifndef FANDATA_H
#define FANDATA_H

#define FAN_HISTORY_SIZE 10

extern int fanStateHistory[FAN_HISTORY_SIZE];
extern int fanHistoryIndex;

void updateFanStateHistory(int newState);
int totalOfMostRecentFanStates();
bool isFanRunningTooMuch(int triggerCount);

void writeFanStateHistoryToEEPROM();
void readFanStateHistoryFromEEPROM();
void writeFanHistoryIndexToEEPROM();
int readFanHistoryIndexFromEEPROM();

void clearFanHistory();

#endif // FANDATA_H