#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

// RFID Pins
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN); 

// Communication Link to Node B
// Arduino A Pin 3 (TX) -> Arduino B Pin 2 (RX)
SoftwareSerial LinkToNodeB(2, 3); // RX, TX

// Soil Sensor Pin
const int soilPin = A0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  
  // ADD THIS LINE FOR TESTING:
  byte v = rfid.PCD_ReadRegister(rfid.VersionReg);
  Serial.print("RFID Firmware Version: 0x");
  Serial.println(v, HEX);

  if (v == 0x00 || v == 0xFF) {
    Serial.println("WARNING: RFID Reader not found! Check wiring.");
  }

  
  Serial.println("NODE A (AUTHENTICATOR) READY");
  Serial.println("Waiting for RFID Scan...");
}

void loop() {
  // 1. Look for new RFID cards
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return; // Stay in loop if no card is scanned
  }

  // 2. Extract the UID from the card
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    // Formatting: adds a leading zero if needed and converts to Hex
    uidString += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  // 3. Read the Soil Moisture (Analog Ao)
  int moistureValue = analogRead(soilPin);

  // 4. DATA SERIALIZATION (The core skill for your report)
  // Create a CSV (Comma Separated Values) packet
  // Format: "UID,MOISTURE"
  String packet = uidString + "," + String(moistureValue);

  // 5. PUBLISH to Node B
  LinkToNodeB.println(packet); // The '\n' tells Node B the message is finished

  // Local Debugging
  Serial.print("Access Granted to: ");
  Serial.println(uidString);
  Serial.print("Sending Telemetry: ");
  Serial.println(packet);
  Serial.println("-------------------------");

  // Halt RFID to prevent multiple reads of the same scan
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  
  delay(2000); // 2-second cooldown before next scan
}