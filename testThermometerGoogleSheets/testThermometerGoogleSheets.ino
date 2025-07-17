#include <WiFi.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>  // ✅ REQUIRED for HardwareSerial

HardwareSerial mySerial(2);  // using UART2 on GPIO16/17


int temperatureThresholdCooling = 72; //change as needed
String isACOverruning = "";


const char* ssid = ""; //Vivo XI
const char* password = "";

const char* googleScriptURL = "https://script.google.com/macros/s/AKfycbz96g48vc5YrUAXp7k1-jZH3-Zy9ShXod-2HFNKMvLIzP2z-IXcCPsp7Wzr8Gh9L2Xh/exec"; // Example: https://script.google.com/macros/s/AKfycbxyz12345/exec

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  mySerial.begin(9600, SERIAL_8N1, 16, 17);  // RX, TX pins

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");

  // Send initial "test" message
  sendToGoogleSheet("test");
}

void loop() {
  if (mySerial.available()) {
    String serialMessage = mySerial.readStringUntil('\n');  // Read until newline
    serialMessage.trim();  // Remove any trailing newline or spaces

    if (serialMessage.length() > 0) {
      sendToGoogleSheet(serialMessage);
    }
  }
}

void sendToGoogleSheet(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(googleScriptURL) + "?message=" + message;
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Google Sheets response: " + payload);
    } else {
      Serial.println("HTTP error: " + String(httpCode));
    }
    
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}