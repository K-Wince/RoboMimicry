// Includes
#include <Arduino.h>
#include "WiFi.h"
#include "esp_now.h"

// Defines and Constants
const int numPots = 4;

// RECEIVER MAC ADDRESS - C0:49:EF:E4:54:6C
uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0xE4, 0x54, 0x6C};
esp_now_peer_info_t peerInfo; // Initialize an object for the peer info
int myData[numPots + 2]; // Create the data packet

// Define the ADC inputs
int potPins[numPots] = {36, 39, 34, 35};

#define but1 27
#define but2 26


void setup(){
  // Initialize Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receiver
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add the transmitter       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Initialize the User Interface
  pinMode(18, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
}
 
void loop() {
  // Read the Potentiometers
  for(int i = 0; i < numPots; i++){
    myData[i] = (int)(analogRead(potPins[i]) * 180/4095);
  }

  myData[4] = 90; 

  // Check whether if they want to close the claw or not
  if (digitalRead(but1) == 1) {
    myData[5] = 1;
  } else if (digitalRead(but2) == 1) {
    myData[5] = 2;
  } else {
    myData[5] = 0;
  }

  // Send the data packet
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));    
}