#ifndef ESP_NOW_H
#define ESP_NOW_H

#include <esp_now.h>
#include <WiFi.h>
    
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //MAC Address of the destination board 
String status; // outgoing message
float shockPosition; //  |incoming messages
float pitch;         //  |
float yaw;           //  |
float roll;          //  |

String success; // to store if data was successfully sent
typedef struct struct_message {
    // Must match the receiver structure.
    bool status;
    float shockPosition;
    //float pitch;
    //float yaw;
    //float roll;
} struct_message;

// Create a struct_message to hold outgoing & incoming messages
struct_message outgoingReadings;
struct_message incomingReadings;

esp_now_peer_info_t peerInfo;

void SendData(){
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgoingReadings, sizeof(outgoingReadings));
}

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(state == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (state == 0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  //example function written for the central module (REDEFINE IT FOR EACH MODULE)
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  shockPosition = incomingReadings.shock;

  pitch = incomingReadings.pitch;
  yaw = incomingReadings.yaw;
  roll = incomingReadings.roll;
}

void initialise_ESP_NOW(){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

#endif