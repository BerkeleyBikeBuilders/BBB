#include <esp_now.h>
#include <WiFi.h>

// Define receiver MAC address
uint8_t receiverMACAddress[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

// Define message to send
String messageToSend = "Hello, Receiver!";

// Function to handle data sent callback
void onDataSent(uint8_t* macAddr, uint8_t sendStatus) {
  if (sendStatus == 0) {
    Serial.println("Message sent successfully!");
  } else {
    Serial.print("Error sending message. Error code: ");
    Serial.println(sendStatus);
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }

  // Register callback for data sent
  esp_now_register_send_cb(onDataSent);

  // Add receiver to peer list
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMACAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Send message to receiver
  esp_err_t result = esp_now_send(receiverMACAddress, (uint8_t*)messageToSend.c_str(), messageToSend.length());
  if (result == ESP_OK) {
    Serial.println("Message sent to receiver: " + messageToSend);
  } else {
    Serial.println("Failed to send message");
  }

  delay(1000); // Wait for 1 second before sending next message
}
