#include <WiFi.h>
#include <esp_now.h>
#include <HardwareSerial.h>  // ✅ REQUIRED for HardwareSerial

HardwareSerial mySerial(2);  // using UART2 on GPIO16/17

// Callback when data is received
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           recv_info->src_addr[0], recv_info->src_addr[1], recv_info->src_addr[2],
           recv_info->src_addr[3], recv_info->src_addr[4], recv_info->src_addr[5]);

  Serial.print("Received from ");
  Serial.print(macStr);
  Serial.print(" (");
  Serial.print(len);
  Serial.print(" bytes): ");

  String message = "";
  for (int i = 0; i < len; i++) {
    message += (char)incomingData[i];
  }
  Serial.println(message);
  mySerial.println(message);
}

void setup() {
  Serial.begin(115200);

  mySerial.begin(9600, SERIAL_8N1, 16, 17);  // RX, TX pins

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println("ESP-NOW initialized. Waiting for messages...");

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Nothing needed in loop
}
