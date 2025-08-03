#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_SHT31.h> // Uncommented SHT31
#include <DHT.h> // Commented out DHT11

#include "SequenceToApartmentNumberCopy.h"
#include "UserPrompt.h"
#include "Globals.h"
#include "DeviceTypes.h"
#include "FanData.h"
#include "Microphone.h"

//IMPORTANT SETUP BELOW!!!!!!!!!!!!!!!!!!!!!

  //go to setup() to comment in the initializer for fan sensors if needed
  const int tempCalibration = 0;  //change as needed


//int sequenceNumber = originalUnitNumber;
float temperature;
float humidity;
int isFanOn = 0; 
unsigned long lastSendTime = 0;
unsigned long waitTime = 0;

int LED_PIN; // 2 or 15 depending on the board
const int SDA_PIN = 21; //33
const int SCL_PIN = 22; //35
const int BUTTON_PIN = 33; //only used for fan sensor setups


//Adafruit_SHT31 sht31 = Adafruit_SHT31(); // Uncommented SHT31

 #define DHTPIN 4      // DHT11 data pin (change as needed)
 #define DHTTYPE DHT11 // DHT 11 or 22
 DHT dht(DHTPIN, DHTTYPE); // DHT11 sensor instance

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Updates the incoming message by replacing its prefix with the current sequence number
String updateIncomingSequenceNumber(const String& received, const String& prefix) {
  String modifiedMsg = String(sequenceNumber);
  modifiedMsg += received.substring(prefix.length());
  return modifiedMsg;
}

void broadcastData() {
  if (isFanSensor == 0) {
    //temperature = sht31.readTemperature() * 1.8 + 32 + tempCalibration;
    //humidity = sht31.readHumidity();

    // DHT11 sensor reading
     float t = dht.readTemperature(); // Celsius
     float h = dht.readHumidity();    // Read humidity
     temperature = t * 1.8 + 32 + tempCalibration; // Convert to Fahrenheit
     humidity = h;
  }
  if (isFanSensor == 1) {
    temperature = 00;
    humidity = 00;
    // REMOVE THIS LINE:
    // readMicrophoneLevel(); // This will set isFanOn based on microphone input
    
    // isFanOn is already set by the call in loop() - don't read it again!
  }
  
  if (isInitializerForFanSensor == 1) {
    isASetupBroadcast = 1; // Set flag for setup broadcast
  } else {
    isASetupBroadcast = 0; // Reset flag for regular broadcasts
  }

  String messageStr = String(sequenceNumber) + "," + String((int)round(temperature)) + "," + String((int)round(humidity)) + "," + String(originalUnitNumber) + "," + String((int)isFanOn) + "," + String((int)isFanSensor)+ "," + String((int)isASetupBroadcast);//isASetupBroadcast must be the last element in the message for now
  const char* message = messageStr.c_str();

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)message, strlen(message));

  if (result == ESP_OK) {
    Serial.print("Sent message: ");
    Serial.println(messageStr);
  } else {
    Serial.println("Error sending the message");
  }
}

void sendTestMessage() {
  const char *message = "test";
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)message, strlen(message));

  if (result == ESP_OK) {
    Serial.println("Sent message: test");
  } else {
    Serial.println("Error sending message");
  }
}



void setupAndBlinkLED(int pin, int numBlinks) {
  pinMode(pin, OUTPUT);
  for (int i = 0; i < numBlinks; i++) {
    digitalWrite(pin, HIGH);
    delay(200);
    digitalWrite(pin, LOW);
    delay(200);
  }
}

// --------- Callback Function ---------
void onReceive(const uint8_t *mac, const uint8_t *incomingData, int len) {
  String received = "";
  for (int i = 0; i < len; i++) {
    received += (char)incomingData[i];
  }
  Serial.println("Received message: " + received);

  if (received.length() >= 2) {
    String prefix = received.substring(0, 2);
    int prefixNum = prefix.toInt();

    int lowerBound = sequenceNumber;
    if (isFloorLinker == 1) {
      lowerBound = sequenceNumber - 1;
    } else if (isFloorSender == 1) {
      lowerBound = sequenceNumber - 3;
    }

    // Always repeat if sender is from the same unit (fan sensor message)
    if (prefixNum == originalUnitNumber && isFanSensor == 0) {
      Serial.println("Repeating message from same unit (fan sensor): " + String(prefixNum));
      String modifiedMsg = updateIncomingSequenceNumber(received, prefix);
      esp_now_send(broadcastAddress, (uint8_t *)modifiedMsg.c_str(), modifiedMsg.length());
      setupAndBlinkLED(2,2);
    }
    // Otherwise, use the original lowerBound logic
    else if (prefixNum >= lowerBound && prefixNum < sequenceNumber && isFanSensor == 0) {
      Serial.println("Received valid prefix: " + String(prefixNum));
      delay(1000);

      String modifiedMsg = updateIncomingSequenceNumber(received, prefix);
      esp_now_send(broadcastAddress, (uint8_t *)modifiedMsg.c_str(), modifiedMsg.length());
      setupAndBlinkLED(2,2);

    } else {
      Serial.println("Ignored message with prefix: " + String(prefixNum));
    }
  }
}

// Temporary callback to update sequenceNumber
void tempReceiveCallback(const uint8_t *mac, const uint8_t *incomingData, int len) {
  String received = "";
  for (int i = 0; i < len; i++) {
    received += (char)incomingData[i];
  }
  int lastCommaIndex = received.lastIndexOf(',');
  char lastValue = received.charAt(lastCommaIndex + 1);
  if (lastValue == '1') {
    int incomingSeq = received.substring(0, received.indexOf(',')).toInt();
    sequenceNumber = incomingSeq;
    originalUnitNumber = sequenceNumber;
    EEPROM.write(0, originalUnitNumber);  // Save to EEPROM
    EEPROM.commit();
    Serial.print("Unit number updated to: ");
    Serial.println(originalUnitNumber);
    setupAndBlinkLED(15, 5);  //ESP32s2 ONLY!!!!
    broadcastData();
  }

}

 void broadcastFanStateHistory() {
      String historyStr = "";
      for (int i = 0; i < FAN_HISTORY_SIZE; i++) {
        historyStr += String(fanStateHistory[i]);
        if (i < FAN_HISTORY_SIZE - 1) historyStr += ",";
      }
      esp_now_send(broadcastAddress, (uint8_t *)historyStr.c_str(), historyStr.length());
      setupAndBlinkLED(15, 6); // ESP32s2 ONLY!!!!
      Serial.print("Broadcasted fan state history: ");
      Serial.println(historyStr);
    }




// --------- Setup --------------------------------------------------------------------------------------------------------------------------
void setup() {
  
  EEPROM.begin(512); // Initialize EEPROM with 512 bytes
  




  //isInitializerForFanSensor = true;  // comment in if this is the initializer for fan sensors (it should be set to false under global variables in Globals.h)
  
  // BUG FIX: Force fan sensor mode override (uncomment to enable fan sensor on any ESP32 board)
  // bool forceFanSensorMode = true;  // UNCOMMENT THIS LINE TO FORCE FAN SENSOR MODE ON ESP32_DEV_BOARD
  







  Serial.begin(115200);
  #ifdef CONFIG_IDF_TARGET_ESP32S2
    boardType = "ESP32S2_DEV_BOARD";
    Serial.println("Running on ESP32-S2!");//technically we won't be able to see this
    pinMode(33, INPUT); // button pin for ESP32-S2
  #else
    boardType = "ESP32_DEV_BOARD";
    Serial.println("not esp32s2 mini!");
  #endif

  
  originalUnitNumber = EEPROM.read(0);  // Load saved unit number
  readFanStateHistoryFromEEPROM(); // Load fan state history from EEPROM and save it in the array called fanStateHistory[]
  fanHistoryIndex = readFanHistoryIndexFromEEPROM(); // Load the index for the fan state history from EEPROM
  

  Wire.begin(SDA_PIN, SCL_PIN);
/*
  if (!sht31.begin(0x44)) {
    Serial.println("Couldn't find SHT31 sensor!");
  } else {
    Serial.println("SHT31 sensor found.");
  }
*/

  dht.begin(); // Initialize DHT22 sensor
  Serial.println("DHT22 sensor initialized.");


  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  Serial.print("isFanSensor value: ");
  Serial.println(isFanSensor);
  if (isFanSensor == 0) {
    esp_now_register_recv_cb(onReceive);
    Serial.println("Listening for incoming messages...");
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add broadcast peer");
    return;
  }

  //MAIN xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
  sequenceNumber = originalUnitNumber;

  // BUG FIX: Check for force fan sensor mode override
  bool shouldBeFanSensor = false;
  #ifdef FORCE_FAN_SENSOR_MODE
    shouldBeFanSensor = true;
    Serial.println("FORCE_FAN_SENSOR_MODE: Enabling fan sensor mode");
  #else
    // Check for local override (user can uncomment the line above)
    #ifdef forceFanSensorMode
      if (forceFanSensorMode) {
        shouldBeFanSensor = true;
        Serial.println("Local override: Enabling fan sensor mode on ESP32_DEV_BOARD");
      }
    #endif
  #endif
  
  if (boardType == "ESP32_DEV_BOARD" && !shouldBeFanSensor) {         //should be a standard device, not a fan sensor
    setupAndBlinkLED(2, 3);

    while(isInitializerForFanSensor == 1) { //infinite loop until user input
      Serial.println("This is the initializer for fan sensors. Please enter the sequence number to broadcast:");
      promptUser(); 
      broadcastData();
    }
    isASetupBroadcast = 0; // might not be needed 
    promptUser(); 
    broadcastData();
      
    displayApartmentNumber();
    setDeviceType(originalUnitNumber);
    printDeviceType();



  } else {                                       //should be a fan sensor OR forced fan sensor mode
    setupAndBlinkLED(15, 4);
    isFanSensor = 1;
    
    // Setup microphone pin
    pinMode(MIC_PIN, INPUT);
    
    unsigned long listenStart = millis();
    unsigned long listenDuration = 10000; // 10 seconds

    // BUG FIX: Always calibrate microphone to ensure accurate readings
    calibrateMicrophone(); // Calibrate microphone baseline every time
    
    // Only do the following if device was manually reset (not from deep sleep wakeup)
    if (esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_TIMER) {
      clearFanHistory();                                            //probably want to comment this out when testing
      esp_now_register_recv_cb(tempReceiveCallback);
      broadcastData();
      digitalWrite(15, HIGH); // Turn on LED to indicate listening
      while (millis() - listenStart < listenDuration) {
        delay(10); // allow time for callbacks
      }
      digitalWrite(15, LOW); // Turn off LED after listening
      esp_now_unregister_recv_cb();
    }

    //esp_sleep_enable_timer_wakeup(5 * 60 * 1000000ULL); // 5 minutes in microseconds
    //Serial.println("Going to sleep...");
    //esp_deep_sleep_start();
  }

  

  // Uncomment for testing:
  // sendTestMessage();

}




// --------- Loop ---------
void loop() {
  // BUG FIX: Use else if structure and add error handling for invalid isFanSensor values
  if (isFanSensor == 0) {
    // Temperature sensor mode
    if (millis() - lastSendTime >= waitTime) {
      broadcastData();
      lastSendTime = millis();
      int waitTimeMinMinutes = 8; // Lower limit in minutes
      int waitTimeMaxMinutes = 12; // Upper limit in minutes
      waitTime = random(waitTimeMinMinutes * 60000, (waitTimeMaxMinutes + 1) * 60000);
      Serial.print("Next wait time in minutes: ");
      Serial.println(waitTime / 60000);
    }
  } else if (isFanSensor == 1) {
    // Fan sensor mode
    // Read microphone level and set isFanOn based on sound
    readMicrophoneLevel();
    
    updateFanStateHistory(isFanOn); // Update the fan state history array
    writeFanStateHistoryToEEPROM(); // Write the updated fan state history to EEPROM
    broadcastFanStateHistory(); // For testing only
    if(isFanRunningTooMuch(5)){
      delay(100); // Wait for 100 milliseconds to avoid rapid re-broadcasting
      broadcastData();
      delay(500); // Wait for 500 milliseconds before going to sleep
    }
    esp_sleep_enable_timer_wakeup(0.25 * 60 * 1000000ULL); // 0.25 minutes in microseconds
    esp_deep_sleep_start();
  } else {
    // BUG FIX: Handle invalid isFanSensor values
    Serial.print("ERROR: Invalid isFanSensor value: ");
    Serial.println(isFanSensor);
    Serial.println("Resetting to temperature sensor mode (0)");
    isFanSensor = 0;
    delay(1000); // Prevent rapid error messages
  }
}


