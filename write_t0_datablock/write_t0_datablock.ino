#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Create a new key
MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Ready to write...");

  // Prepare the default key for authentication
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;  // Default key is FFFFFFFFFFFFh
  }
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("Card detected! UID:");
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

    String studentID = "LANDMARK25-EPS005";
    byte newData[16];
    studentID.getBytes(newData, 16);

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

    // Write the data to the block
    status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockNum, newData, 16);

    // Check if the write operation was successful
    if (status == MFRC522::STATUS_OK) {
      Serial.println("Data successfully written!");
    } else {
      Serial.print("Writing failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
    }

    // Now, read the data back for verification
    byte readData[18];
    byte bufferSize = sizeof(readData);
    status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockNum, readData, &bufferSize);
    if (status == MFRC522::STATUS_OK) {
      Serial.println("Data read successfully!");

      String readString = "";
      for (int i = 0; i < 16; i++) {
        readString += (char)readData[i];
      }
      Serial.print("The ID on the tag is: ");
      Serial.println(readString);
    } else {
      Serial.print("Reading failed: ");
      Serial.println(mfrc522.GetStatusCodeName(status));
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}