Smart Thermometer Network
A distributed IoT network system using ESP32 devices for temperature monitoring and fan detection in multi-unit buildings (apartments/condos).

Overview
This system creates a mesh network of ESP32 devices that collect temperature/humidity data and monitor HVAC fan activity. Data is relayed through the building using ESP-NOW protocol to reach a central receiver connected to the internet.

Device Types

1. Temperature Sensors (ESP32 Dev Board)
Purpose: Monitor temperature and humidity in individual units
Hardware: ESP32 Dev Board + DHT11/DHT22 sensor
Features:
Reads temperature/humidity every 8-12 minutes (randomized)
Broadcasts data to neighboring devices
Repeats messages from other units to extend network range
Stores unit number in EEPROM for persistence

2. Fan Sensors (ESP32-S2 Mini)
Purpose: Monitor HVAC fan activity using acoustic detection
Hardware: ESP32-S2 Mini + MAX4466 microphone module
Features:
Detects fan noise using sound level analysis
Tracks fan state history (last 10 readings) in EEPROM
Wakes every 15 seconds to sample audio
Broadcasts alert when fan runs excessively (5+ out of 10 readings)
Deep sleep mode for battery conservation

There are two other sketches. One transfers these readings from espnow to serial, and the other takes serial and puts it on a Google sheet
