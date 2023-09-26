#include <Arduino.h>

#include "WiFi.h"
#include "esp_now.h"
#include "Servo.h"

// Create the data packet
int myData[6];

// Create the servos
int servoPins[5] = {32, 33, 26, 27, 35};
int currServoPos[5] = {90, 90, 90, 90, 90};

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;


// Create the pins for claw
int clawPin = 25;
int valvePin = 13;

// Create a callback function on receiving data
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // for (int i = 0; i < 4; i++;) {
  //   servoPos[i] = myData[i];
  // }
  // servo1.write(myData[0]);
  // servo2.write(myData[1]);
  // servo3.write(myData[2]);
  // servo4.write(myData[3]);
  // servo5.write(myData[4]);
  //Serial.printf("myData: %d ", myData[1]);

  for (int i = 0; i < 5; i++) {
      currServoPos[i] += ((myData[i] - currServoPos[i]) / 2);

    switch(i) {
        case 0: servo1.write(currServoPos[i]); break;
        case 1: servo2.write(currServoPos[i]); break;
        case 2: servo3.write(currServoPos[i]); break;
        case 3: servo4.write(currServoPos[i]); break;
        case 4: servo5.write(currServoPos[i]); break;
    }
  } 

  if (myData[5] == 1) {
    digitalWrite(valvePin, HIGH);
    digitalWrite(clawPin, HIGH);
  } else if (myData[5] == 2) {
    digitalWrite(valvePin, LOW);
    digitalWrite(clawPin, LOW);
  } else {
    digitalWrite(valvePin, HIGH);
    digitalWrite(clawPin, LOW);
  }
}
 
void setup(){
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Attach the servo
  servo1.attach(servoPins[0]);
  servo2.attach(servoPins[1]);
  servo3.attach(servoPins[2]);
  servo4.attach(servoPins[3]);
  servo5.attach(servoPins[4]);

  pinMode(valvePin, OUTPUT);
  pinMode(clawPin, OUTPUT);

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop(){
  //for(int i = 0; i < 5; i++) {
  //   Serial.print(i);
  //   Serial.print(": ");
  //   Serial.print(myData[i]);
  //   Serial.print("  ");

    // if (myData[i] == currServoPos[i]) {
    //   return;
    // } else if (myData[i] > currServoPos[i]) {
    //   currServoPos[i] += 1;     
    // } else if (myData[i] < currServoPos[i]) {
    //   currServoPos[i] -= 1;
    // }
    

    // switch(i) {
    //     case 0: servo1.write(currServoPos[i]); break;
    //     case 1: servo2.write(currServoPos[i]); break;
    //     case 2: servo3.write(currServoPos[i]); break;
    //     case 3: servo4.write(currServoPos[i]); break;
    //     case 4: servo5.write(currServoPos[i]); break;
    //   }
  //} 

  // Serial.printf("curr: %d \n", currServoPos[1]);
}