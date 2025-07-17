#include "DeviceTypes.h"
#include "Globals.h"
#include <Arduino.h>

void setDeviceType(int sequenceEntry){
    if ((sequenceNumber - 2) % 4 == 0 && sequenceNumber != 46) { // 46, the last unit, does not need to repeat anymore
        isFloorSender = 1;
        isFloorLinker = 0;
    } else if ((sequenceNumber - 3) % 4 == 0) {
        isFloorSender = 0;
        isFloorLinker = 1;
    } else {
        isFloorSender = 0;
        isFloorLinker = 0;
    }
}

void printDeviceType() {
    if (isFloorSender) {
        Serial.println("Device is a Column Gatherer");
    } else if (isFloorLinker) {
        Serial.println("Device is a Column Linker");
    } else if (isInitializerForFanSensor) {
        Serial.println("Device is an Initializer for Fan Sensor");
    } else {
        Serial.println("Device is a Regular Unit");
    }
}

