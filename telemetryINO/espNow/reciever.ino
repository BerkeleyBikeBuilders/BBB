#include <esp_now.h>
#include <WiFi.h>

// Define controller MAC address
uint8_t controllerMACAddress[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

// Function to handle data received callback
void onDataReceived(uint8_t* macAddr, uint8_t* data, int dataLen) {
  Serial.print("Message received from controller: ");
  Serial.println((char*)data);

  // Send continuous messages back to controller
  esp_now_send(macAddr, data, dataLen);
}

void setup() {
  Serial.begin(115200);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }

  // Register callback for data received
  esp_now_register_recv_cb(onDataReceived);

  // Add controller to peer list
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, controllerMACAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  delay(1000); // Continuous sending is handled in the onDataReceived callback
}
