#include "SequenceToApartmentNumber.h"
#include <Arduino.h>

// Conversion function for apartment unit chart
// Returns the apartment string for a given unit number


String sequenceToApartmentNumber(int sequenceNumber) {
    switch (sequenceNumber) {
    case 11: return "101";
    case 12: return "102";
    case 13: return "202";
    case 14: return "201";
    case 15: return "301";
    case 16: return "302"; // *a halfTowerRepeater
    case 17: return "304"; // *b miniHalfTowerRepeater
    case 18: return "303"; 
    case 19: return "203";
    case 20: return "204";
    case 21: return "104";
    case 22: return "103"; // *a halfTowerRepeater
    case 23: return "105"; // *b miniHalfTowerRepeater
    case 24: return "106";
    case 25: return "206";
    case 26: return "205";
    case 27: return "305";
    case 28: return "306"; // *a halfTowerRepeater
    case 29: return "308"; // *b miniHalfTowerRepeater
    case 30: return "307";
    case 31: return "207";
    case 32: return "208";
    case 33: return "108";
    case 34: return "107";
    default: return "";
  }
}