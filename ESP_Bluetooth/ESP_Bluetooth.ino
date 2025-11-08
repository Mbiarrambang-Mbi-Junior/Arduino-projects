#include <BluetoothSerial.h>

// set bluetooth object
BluetoothSerial ESP_BT;

// initialise pins
// NOTE: GPIO 0 and GPIO 2 are critical for ESP32 boot mode. 
// Using different pins (e.g., 17 and 18) is highly recommended to avoid upload issues.
int YELLOWPIN = 17;
int BLUEPIN   = 16;
int REDPIN    = 18;
int GREENPIN  = 4;

// Function to turn all pins on
void AllON() {
  Serial.println("All leds are On");
  digitalWrite(YELLOWPIN, HIGH);
  digitalWrite(REDPIN, HIGH);
  digitalWrite(BLUEPIN, HIGH);
  digitalWrite(GREENPIN, HIGH);
}

// Function turns all leds off
void AllOFF() {
  Serial.println("All leds are Off");
  digitalWrite(YELLOWPIN, LOW);
  digitalWrite(REDPIN, LOW);
  digitalWrite(BLUEPIN, LOW);
  digitalWrite(GREENPIN, LOW);
}

void setup() {
  // start serial monitor
  Serial.begin(115200);

  // assign pin modes
  pinMode(YELLOWPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);

  // by default all pins are low
  digitalWrite(YELLOWPIN, LOW);
  digitalWrite(REDPIN, LOW);
  digitalWrite(BLUEPIN, LOW);
  digitalWrite(GREENPIN, LOW);

  // set bluetooth name
  ESP_BT.begin("ESP32_Control");
  Serial.println("Bluetooth is now discoverable as ESP32_Control");
  // Updated prompt message to reflect the new command structure (1-4 ON, 5-8 OFF, 9/0 ALL)
  Serial.println("Send commands: 1-4 (ON) or 5-8 (OFF). Use 9 for ALL ON, 0 for ALL OFF.");
}

void loop() {
  // checking for bluetooth availability
  if (ESP_BT.available()) {
    // read characters recieved from bluetooth
    char incomingChar = ESP_BT.read();

    Serial.print("Received: ");
    Serial.println(incomingChar);

    // checking for characters from bluetooth
    switch (incomingChar) {
      
      // --- ALL ON COMMANDS (1, 2, 3, 4) ---
      case '1':
        Serial.println("Button 1 ON: YELLOW LED");
        digitalWrite(YELLOWPIN, HIGH);
        break;
      case '2':
        Serial.println("Button 2 ON: RED LED");
        digitalWrite(REDPIN, HIGH);
        break;
      case '3':
        Serial.println("Button 3 ON: BLUE LED");
        digitalWrite(BLUEPIN, HIGH);
        break;
      case '4':
        Serial.println("Button 4 ON: GREEN LED");
        digitalWrite(GREENPIN, HIGH);
        break;

      // --- ALL OFF COMMANDS (5, 6, 7, 8) ---
      case '5':
        Serial.println("Button 1 OFF: YELLOW LED"); 
        digitalWrite(YELLOWPIN, LOW);
        break;
      case '6':
        Serial.println("Button 2 OFF: RED LED"); 
        digitalWrite(REDPIN, LOW);
        break;
      case '7':
        Serial.println("Button 3 OFF: BLUE LED");
        digitalWrite(BLUEPIN, LOW);
        break;
      case '8':
        Serial.println("Button 4 OFF: GREEN LED");
        digitalWrite(GREENPIN, LOW);
        break;

      // --- ALL ON/OFF COMMANDS ---
      case '9':
        // turn all leds on
        AllON();
        break;
      case '0':
        // turn all leds to off
        AllOFF();
        break;
        
      // the error message for invalid commands
      default:
        ESP_BT.println("Invalid Command");
        break;
    }
  }
  // gives a delay
  delay(20);
}