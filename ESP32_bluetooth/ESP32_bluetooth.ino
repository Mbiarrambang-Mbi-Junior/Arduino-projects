#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

const int relayPins[4] = {23, 22, 21, 19};
const int switchPins[4] = {18, 5, 17, 16};
bool relayState[4] = {false, false, false, false};

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32-HomeAuto");

  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    pinMode(switchPins[i], INPUT_PULLUP); // Use pull-up mode
    digitalWrite(relayPins[i], LOW); // All relays off
  }
}

void loop() {
  // Bluetooth command handling
  if (SerialBT.available()) {
    char cmd = SerialBT.read();

    switch (cmd) {
      case 'A':
      case 'a':
        toggleRelay(0);
        break;
      case 'B':
      case 'b':
        toggleRelay(1);
        break;
      case 'C':
      case 'c':
        toggleRelay(2);
        break;
      case 'D':
      case 'd':
        toggleRelay(3);
        break;
    }
  }

  // Manual switch handling
  for (int i = 0; i < 4; i++) {
    if (digitalRead(switchPins[i]) == LOW) { // Active LOW
      delay(50); // debounce
      if (digitalRead(switchPins[i]) == LOW) {
        toggleRelay(i);
        while (digitalRead(switchPins[i]) == LOW); // wait for release
        delay(50); // debounce on release
      }
    }
  }
}

void toggleRelay(int index) {
  relayState[index] = !relayState[index];
  digitalWrite(relayPins[index], relayState[index]);
}