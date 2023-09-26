// Includes
#include <Arduino.h>
#include "WiFi.h"
#include "esp_now.h"
#include <LiquidCrystal_I2C.h>

// Defines and Constants
const int numPots = 5;

// RECEIVER MAC ADDRESS - C0:49:EF:E4:54:6C
uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0xE4, 0x54, 0x6C};
esp_now_peer_info_t peerInfo; // Initialize an object for the peer info
int myData[numPots + 1]; // Create the data packet
 
// Define the user interface
LiquidCrystal_I2C lcd(0x27,20,4);  // set the addr 0x27 for a 20 chars and 4 lines
int buttonPins[4] = {};
int rotaryPins[2] = {};
int currMenu = 0;
int currMode = 0;
int currSelec = 1;
int currMap = 1;

// Define the ADC inputs
int potPins[numPots] = {36, 39, 34, 35, 32};

// Define the Motion Map Presets
uint8_t currentPos = 0;
uint8_t currentPreset = 1;
int preset1[144];
int preset2[144];
int preset3[144];
int preset4[144];

// Define the User Input Vars
int lastAIn = 0;
int currAIn = 0;
unsigned long lastButton = 0;

#define AChan 4
#define BChan 17
#define potBut 16
#define but1 25
#define but2 26
#define but3 27
#define but4 14


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
  lcd.init();
  lcd.clear();
  lcd.backlight();

  
  pinMode(AChan, INPUT);
  pinMode(BChan, INPUT);
  pinMode(but1, INPUT_PULLUP);
  pinMode(but2, INPUT_PULLUP);
  pinMode(but3, INPUT_PULLUP);
  pinMode(but4, INPUT_PULLUP);
  pinMode(potBut, INPUT_PULLUP);

  lastAIn = digitalRead(AChan);
}
 
void loop(){
  // Take in the Rotary Encoder and see which dir it is turning
  currAIn = digitalRead(AChan);
  if(currAIn != lastAIn && currAIn == 1){
    if(digitalRead(BChan) != currAIn){ currSelec++; } 
    else{ currSelec--; }

    if (currSelec == 5) { currSelec = 1; }
    else if (currSelec == 0) { currSelec = 4; }
  }

  // MODE = MENU
  if (currMode == 0) {
    if (currMenu == 0) { // Main Selection Menu
      // Create the Direct Control Option
      lcd.setCursor(0,0);
      if (currSelec % 2 == 1) { lcd.print("- ");} 
      else { lcd.print("  "); }
      lcd.print("Direct Control");

      // Create the Motion Map Option
      lcd.setCursor(0,1);
      if (currSelec % 2 == 0) { lcd.print("- ");} 
      else { lcd.print("  "); }
      lcd.print("Motion Map");

      // Check whether the user has selected or not
      if (digitalRead(potBut) == 0 && (millis() - lastButton) > 500) {
        if(currSelec % 2 == 1) {
          currMode = 1;
        } else if (currSelec % 2 == 0) {
          currMenu = 1;
          currSelec = 1;
        }
        lcd.clear();
        lastButton = millis();
      }
    } else if (currMenu == 1) { // Motion Map Menu
      // Create the Map 1
      lcd.setCursor(0,0);
      if (currSelec == 1) { lcd.print("- ");} 
      else { lcd.print("  "); }
      lcd.print("Map 1");

      // Create the Map 2
      lcd.setCursor(0,1);
      if (currSelec == 2) { lcd.print("- ");} 
      else { lcd.print("  "); }
      lcd.print("Map 2");

      // Create the Map 3
      lcd.setCursor(0,2);
      if (currSelec == 3) { lcd.print("- ");} 
      else { lcd.print("  "); }
      lcd.print("Map 3");

      // Create the Map 4
      lcd.setCursor(0,3);
      if (currSelec == 4) { lcd.print("- ");} 
      else { lcd.print("  "); }
      lcd.print("Map 4");

      // Check whether the user has selected or not
      if (digitalRead(potBut) == 0 && (millis() - lastButton) > 500) {
        currMap = currSelec;
        currMenu = 2;
        currSelec = 1;
        lcd.clear();
        lastButton = millis();
      } else if (digitalRead(but1) == 1 && (millis() - lastButton) > 500) {
        currMenu = 0;
        currSelec = 1;
        lcd.clear();
        lastButton = millis();
      }
    } else if (currMenu == 2) { // Motion Selection
      // Create the Map Selected
      lcd.setCursor(0,0);
      lcd.print("Map ");
      switch(currMap) {
        case 1: lcd.print("1"); break;
        case 2: lcd.print("2"); break;
        case 3: lcd.print("3"); break;
        case 4: lcd.print("4"); break;
      }

      // Create the Play Selection
      lcd.setCursor(0,1);
      if (currSelec % 2 == 1) { lcd.print("- ");} 
      else { lcd.print("  "); }
      lcd.print("Play");

      // Create the Record Selection
      lcd.setCursor(0,2);
      if (currSelec % 2 == 0) { lcd.print("- ");} 
      else { lcd.print("  "); }
      lcd.print("Record");

      if (digitalRead(potBut) == 0 && (millis() - lastButton) > 500) {
        if (currSelec % 2 == 1) {
          currMode = 3;
        } else if (currSelec % 2 == 0) {
          currMode = 2;
        }
        currSelec = 1;
        lcd.clear();
        lastButton = millis();
      } else if (digitalRead(but1) == 1 && (millis() - lastButton) > 500) {
        currMenu = 1;
        currSelec = 1;
        lcd.clear();
        lastButton = millis();
      }
    }
  } else if (currMode == 1) { // MODE = Direct Control
    // Set up Screen
    lcd.setCursor(0,0);
    lcd.print("Direct Control");

    // Read the Potentiometers
    for(int i = 0; i < numPots; i++){
      myData[i] = (int)(analogRead(potPins[i]) * 180/4095);
    }

    // Check whether if they want to close the claw or not
    if (digitalRead(but3) == 1) {
      myData[5] = 1;
    } else if (digitalRead(but4) == 1) {
      myData[5] = 2;
    } else {
      myData[5] = 0;
    }

    // Send the data packet
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    // IF STOP BUTTON PRESS
    if (digitalRead(but1) == 1 && (millis() - lastButton) > 500) {
      currMode = 0;
    }
  } else if (currMode == 2) { // MODE = Motion Map Record
    // Print on the screen that it is recording
    lcd.setCursor(0,0);
    lcd.print("Recording");

    lcd.setCursor(0,1);
    lcd.print("Map: ");
    lcd.printf("%d / 24", (int)currentPos/6);

    if (digitalRead(but2) == 1 && (millis() - lastButton) > 500) {
      for(int i = 0; i < numPots; i++){
        switch (currentPreset){
          case 1: preset1[i + currentPos] = (int)(analogRead(potPins[i]) * 180/4095); break;
          case 2: preset2[i + currentPos] = (int)(analogRead(potPins[i]) * 180/4095); break;
          case 3: preset3[i + currentPos] = (int)(analogRead(potPins[i]) * 180/4095); break;
          case 4: preset4[i + currentPos] = (int)(analogRead(potPins[i]) * 180/4095); break;
        }
      }



      if (digitalRead(but3) == 1) {
        switch (currentPreset){
          case 1: preset1[5 + currentPos] = 1; break;
          case 2: preset2[5 + currentPos] = 1; break;
          case 3: preset3[5 + currentPos] = 1; break;
          case 4: preset4[5 + currentPos] = 1; break;
        }
      } else if (digitalRead(but4)) {
        switch (currentPreset){
          case 1: preset1[5 + currentPos] = 2; break;
          case 2: preset2[5 + currentPos] = 2; break;
          case 3: preset3[5 + currentPos] = 2; break;
          case 4: preset4[5 + currentPos] = 2; break;
        }
      } else {
        switch (currentPreset){
          case 1: preset1[5 + currentPos] = 0; break;
          case 2: preset2[5 + currentPos] = 0; break;
          case 3: preset3[5 + currentPos] = 0; break;
          case 4: preset4[5 + currentPos] = 0; break;
        }
      }

      currentPos += 6;

      lastButton = millis();
    }

    if ((digitalRead(but1) == 1 && (millis() - lastButton) > 500) || currentPos == 144) {
      for (int i = currentPos; i <= 144; i++) {
        switch (currentPreset){
          case 1: preset1[currentPos] = -1; break;
          case 2: preset2[currentPos] = -1; break;
          case 3: preset3[currentPos] = -1; break;
          case 4: preset4[currentPos] = -1; break;
        }
      }
      currentPos = 0;
      currMode = 0;
      lcd.clear();
    }
  } else if (currMode == 3) { // MODE = Motion Map Play
    lcd.setCursor(0,0);
    lcd.print("Playing");

    for(int i = 0; i < 144; i++) {
      switch (currentPreset){
        case 1: myData[i % 5] = preset1[i];
        case 2: myData[i % 5] = preset2[i];
        case 3: myData[i % 5] = preset3[i];
        case 4: myData[i % 5] = preset4[i];
      }

      if (i % 6 == 0) {
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
        delay(1000);
      }

      if ((digitalRead(but1) == 1 && (millis() - lastButton) > 500) || preset1[i] == -1 || preset2[i] == -1 || 
      preset3[i] == -1 || preset4[i] == -1) {
        currMode = 0;
        lcd.clear();
      }
    }
  }
  
   lastAIn = currAIn;
   delay(10);
}