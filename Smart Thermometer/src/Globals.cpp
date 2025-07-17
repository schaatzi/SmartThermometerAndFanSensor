#include "Globals.h"
#include <Arduino.h>

String boardType = " ";

int originalUnitNumber = 10; // Default unit number
int sequenceNumber = 10; // Sequence number for the current device, initialized to 10

int isFloorSender = 0; // floor senders repeat data from the other three units on the floor
int isFloorLinker = 0; // floor linkers repeat data from the floor senders to the next floor (up or down);
//int isColumnGatherer = 0;   // a column is a group of 6 units, gatherers collect data from the column
//int isColumnLinker = 0;     // linkers link columns by repeating data from gatherers to the next column
int isFanSensor = 0;    
//int isInitializerForFanSensor = 0; // esp32 dev board only used to send sequence number to fan sensors over ESP-NOW, since fan sensors cannot use the serial monitor for user input
bool isInitializerForFanSensor = false; // esp32 dev board only used to send sequence number to fan sensors over ESP-NOW, since fan sensors cannot use the serial monitor for user input

int isASetupBroadcast = 0; // Flag to indicate if this is a setup broadcast