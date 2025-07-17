#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
// Add your global includes, variables, and function declarations here

extern String boardType;

extern int originalUnitNumber;
extern int sequenceNumber;

extern int isFloorSender; 
extern int isFloorLinker; 
//extern int isColumnGatherer;
//extern int isColumnLinker;
extern int isFanSensor;
//extern int isInitializerForFanSensor;
extern bool isInitializerForFanSensor;

extern int isASetupBroadcast; // Flag to indicate if this is a setup broadcast

extern int isFanOn; // Add this line if you need it accessible globally

#endif // GLOBALS_H