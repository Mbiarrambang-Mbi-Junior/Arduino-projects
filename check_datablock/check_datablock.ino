#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Create a key for authentication
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Place your card on the reader to read data...");

  // Prepare the default key for authentication
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF; // Default key is FFFFFFFFFFFFh
  }
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card detected!");
    
    // Dump the card's UID to the serial monitor
    Serial.print("UID:");
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

    byte blockNum = 1;

    // Authenticate with the prepared key
    MFRC522::StatusCode status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Authentication failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      return;
    }

    // Read the data from the block
    byte readData[18];
    byte bufferSize = sizeof(readData);
    status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockNum, readData, &bufferSize);

    if (status == MFRC522::STATUS_OK) {
      // Convert the byte array to a human-readable string
      String readString = "";
      for (int i = 0; i < 16; i++) {
        // Stop converting if a null terminator is found, which is standard for C-strings
        if (readData[i] == 0) {
          break;
        }
        readString += (char)readData[i];
      }
      Serial.print("The data on the tag is: ");
      Serial.println(readString);
    } else {
      Serial.print("Reading failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
    }
    
    // Halt the card and stop crypto
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}