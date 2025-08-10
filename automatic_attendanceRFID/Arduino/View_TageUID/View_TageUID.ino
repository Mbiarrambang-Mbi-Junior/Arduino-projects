#include <SPI.h>
#include <MFRC522.h>

// RFID MFRC522 pin definitions
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  digitalWrite(yellow, HIGH);

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

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "); // Format for serial
    Serial.print(mfrc522.uid.uidByte[i], HEX); // Print each byte of the UID in hexadecimal

    // Format for LCD
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uid_display_buffer[buffer_idx++] = '0';
    }
    sprintf(&uid_display_buffer[buffer_idx], "%X", mfrc522.uid.uidByte[i]); // Print hex
    buffer_idx += (mfrc522.uid.uidByte[i] < 0x10 ? 1 : 2); // Advance index by 1 or 2 characters
    if (i < mfrc522.uid.size - 1) {
      uid_display_buffer[buffer_idx++] = ' '; // Add space between bytes
    }
  }
  uid_display_buffer[buffer_idx] = '\0'; // Null-terminate the string

  Serial.println();
  Serial.println("Card Scanned!");

  // Halt PICC
  mfrc522.PICC_HaltA();

  delay(2000); // Display UID for 2 seconds
  Serial.print("Ready to scan!"); // Reset serial message
}
