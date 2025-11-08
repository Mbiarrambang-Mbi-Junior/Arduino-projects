#include <SoftwareSerial.h>
#include <Servo.h>

// Define SoftwareSerial pins for HC-05 communication
// NOTE: These pins are not D0/D1, eliminating the conflict.
const int bluetoothRx = 11; // Connects to HC-05 TX
const int bluetoothTx = 12; // Connects to HC-05 RX
SoftwareSerial bluetooth(bluetoothRx, bluetoothTx);

Servo myservo;

// Your existing pin definitions
int position = 0;
int livingroom = 2;
int doorLock = 9;  // Servo control pin
int bedroom = 10;

char incomingValue = 0;

void setup() {
    Serial.begin(9600);     // ONLY for debugging output to the PC Serial Monitor
    bluetooth.begin(9600); // **HC-05 communication now starts here**
    
    myservo.attach(doorLock);
    pinMode(livingroom, OUTPUT);
    pinMode(bedroom, OUTPUT);
    
    Serial.println("Smart Home System Ready");
}

void loop() {
    // Check the Bluetooth serial port instead of the hardware Serial port
    if (bluetooth.available()) {
        incomingValue = bluetooth.read();

        // ------------------
        // Servo/Door Lock Control
        // ... your servo logic remains the same ...
        if (incomingValue == '6') {
            for (position = 0; position <= 180; position += 1) {
                myservo.write(position);
                delay(15);
            }
            Serial.println("Door UNLOCKED");
        }
        
        if (incomingValue == '7') { 
            for (position = 180; position >= 0; position -= 1) {
                myservo.write(position);
                delay(15);
            }
            Serial.println("Door LOCKED");
        }
        // ------------------

        // Light Controls (Same as before)
        if (incomingValue == '4') {
            digitalWrite(livingroom, HIGH);
            Serial.println("Living Room Light ON");
        }
        if (incomingValue == '1') {
            digitalWrite(livingroom, LOW);
            Serial.println("Living Room Light OFF");
        }
        if (incomingValue == '5') {
            digitalWrite(bedroom, HIGH);
            Serial.println("Bedroom Light ON");
        }
        if (incomingValue == '2') {
            digitalWrite(bedroom, LOW);
            Serial.println("Bedroom Light OFF");
        }
        if (incomingValue == '9') {
            digitalWrite(livingroom, HIGH);
            digitalWrite(bedroom, HIGH);
            Serial.println("ALL LIGHTS ON");
        }
        if (incomingValue == '0') {
            digitalWrite(livingroom, LOW);
            digitalWrite(bedroom, LOW);
            Serial.println("ALL LIGHTS OFF");
        }
    }
}