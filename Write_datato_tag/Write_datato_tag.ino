#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

// Default authentication key for Mifare Classic 1K cards
// Usually all F's (0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF) for new cards
MFRC522::MIFARE_Key key;

// Define the block we want to write to.
// Sector 1 has blocks 4, 5, 6, 7. Let's use Block 4 (first data block in Sector 1).
// Block 0: UID (read-only)
// Block 1, 2: Data blocks in Sector 0
// Block 3: Sector Trailer for Sector 0
// Block 4, 5, 6: Data blocks in Sector 1
// Block 7: Sector Trailer for Sector 1
// ... and so on.
const byte blockNum = 4; // Absolute block number for the target data block

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  // Prepare the default key for authentication.
  // This must be done before every authentication attempt.
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF; // Set all 6 bytes of the key to 0xFF
  }

  Serial.println("Place an RFID card/fob to write data...");
  Serial.println("Waiting for card...");
}

void loop() {
  // Stage 1: Check for New Card Presence
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Stage 2: Read Card UID
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("\nCard UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Print type of card (for example, MIFARE 1K)
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print(F("PICC type: "));
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // --- Write Data Process ---

  // Data to write (16 bytes)
  // You can put any ASCII characters or byte values here.
  // If your string is shorter than 16 bytes, pad with spaces or nulls.
  // If your string is longer, it will be truncated.
  byte newData[16] = {
    0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, // "Hello Wo"
    0x72, 0x6C, 0x64, 0x21, 0x20, 0x20, 0x20, 0x20  // "rld!    " (padded with spaces)
  };
  // Or simply: byte newData[16] = "Hello World!    "; // Auto-pads with nulls if string < 16 bytes

  Serial.print("Attempting to write to block ");
  Serial.print(blockNum);
  Serial.println("...");

  // Stage 3: Authenticate the sector before writing
  // MFRC522::PICC_CMD_MF_AUTH_KEY_A uses Key A for authentication
  MFRC522::StatusCode status;
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    mfrc522.PICC_HaltA(); // Halt PICC on failure
    mfrc522.PCD_StopCrypto1(); // Stop encryption
    return;
  }
  Serial.println(F("Authentication success."));

  // Stage 4: Write data to the block
  status = mfrc522.MIFARE_Write(blockNum, newData, 16); // 16 bytes is the block size

  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  } else {
    Serial.println(F("Data written successfully!"));
    // You can optionally read back to verify
    byte readBackBuffer[18]; // Buffer for reading (16 bytes data + 2 bytes CRC)
    byte bufferSize = sizeof(readBackBuffer);
    status = mfrc522.MIFARE_Read(blockNum, readBackBuffer, &bufferSize);
    if (status == MFRC522::STATUS_OK) {
      Serial.print("Read back from block ");
      Serial.print(blockNum);
      Serial.print(": ");
      for (byte i = 0; i < 16; i++) {
        Serial.write(readBackBuffer[i]); // Print as ASCII characters
      }
      Serial.println();
    } else {
      Serial.print("Failed to read back data: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
    }
  }

  // Stage 5: Halt PICC
  mfrc522.PICC_HaltA(); // Halt PICC (makes the card ready for the next read/write)
  mfrc522.PCD_StopCrypto1(); // Stop encryption (important for next operation)

  delay(2000); // Small delay before next scan
}