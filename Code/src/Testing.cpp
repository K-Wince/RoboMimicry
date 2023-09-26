// Includes
#include <Arduino.h>
#include "WiFi.h"
#include "esp_now.h"
#include "Servo.h"

// Defines and Constants
const int numPots = 5;

// Define the ADC inputs
int potPins[numPots] = {36, 39, 34, 35, 32};

Servo servo1;


void setup(){
  // Initialize Serial Monitor
  Serial.begin(115200);
  servo1.attach(27);
}
 
void loop() {
  // Read the Potentiometers
  for(int i = 0; i < numPots; i++){
    Serial.print(i);
    Serial.print(": ");
    Serial.print(analogRead(potPins[i])* 180/4095);
    Serial.print("  ");
  }
  Serial.println();

  //servo1.write(90);
}