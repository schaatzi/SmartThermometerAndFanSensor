#include "SequenceToApartmentNumberCopy.h"
#include <Arduino.h>

// Conversion function for apartment unit chart
// Returns the apartment string for a given unit number


String sequenceToApartmentNumberCopy(int sequenceNumber) {
    switch (sequenceNumber) {
    case 11: return "311";
    case 12: return "312";
    case 13: return "310";
    case 14: return "309";//floorSender
    case 15: return "209";//floorLink
    case 16: return "210"; 
    case 17: return "212"; 
    case 18: return "211";//floorSender
    case 19: return "111";//floorLink
    case 20: return "112";
    case 21: return "110";
    case 22: return "109";//floorSender
    case 23: return "107";//floorLink
    case 24: return "108";
    case 25: return "106";
    case 26: return "105";//floorSender
    case 27: return "205";//floorLink
    case 28: return "206"; 
    case 29: return "208"; 
    case 30: return "207";//floorSender
    case 31: return "307";//floorLink
    case 32: return "308";
    case 33: return "306";
    case 34: return "305";//floorSender
    case 35: return "303";//floorLink
    case 36: return "304";
    case 37: return "302";
    case 38: return "301";//floorSender
    case 39: return "201";//floorLink
    case 40: return "202";
    case 41: return "204";
    case 42: return "203";//floorSender
    case 43: return "103";//floorLink
    case 44: return "104";
    case 45: return "102";
    case 46: return "101";//preferably not a floorSender
    //the main receiver that is wired to the internet esp32 just need to accept messages from case 43 and higher
    
    //if ((sequenceNumber - 2) % 4 == 0) return "Floor Sender";
    //if ((sequenceNumber - 3) % 4 == 0) return "Floor Linker";

    default: return "";
  }
}