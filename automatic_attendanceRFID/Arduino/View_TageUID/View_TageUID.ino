#include <SPI.h>
#include <MFRC522.h>

// RFID MFRC522 pin definitions for Arduino Uno
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("RFID Scanner - Waiting for card...");
}

void loop() {
  // Check for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("Card UID:");
  // Loop through the 4 bytes of the UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    // Add a leading space and a '0' if the hex value is single digit (e.g., " 0F" instead of " F")
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX); // Print each byte of the UID in hexadecimal
  }

  Serial.println();
  Serial.println("Card Scanned!");

  // Halt PICC (stops the card from being read repeatedly)
  mfrc522.PICC_HaltA();
  
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  delay(2000); // Wait 2 seconds
  Serial.println("Ready to scan!"); 
}