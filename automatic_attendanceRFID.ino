#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>          // Required for I2C communication
#include <LiquidCrystal_I2C.h> // Required for LCD I2C

// RFID MFRC522 pin definitions
#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

// LCD I2C definitions
// Replace 0x27 with the address you found using the I2C scanner (common: 0x27, 0x3F)
// For 16x2 LCD, use LiquidCrystal_I2C(address, 16, 2)
// For 20x4 LCD, use LiquidCrystal_I2C(address, 20, 4)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address and dimensions (e.g., 16 columns, 2 rows)

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  // Initialize the LCD
  lcd.init();      // Initialize the LCD module
  lcd.backlight(); // Turn on the backlight (if available)

  lcd.print("wellcom!!");
  lcd.setCursor(0, 1); // Set cursor to the second line, first column
  lcd.print("Ready to scan!");
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

  // Clear previous messages on LCD
  lcd.clear();
  lcd.setCursor(0, 0); // Start at top left

  Serial.print("Card UID:");
  lcd.print("UID:"); // Display "UID:" on LCD

  // Store UID in a buffer to display nicely on LCD
  char uid_display_buffer[17]; // 4 bytes * 2 chars/byte + 3 spaces + null terminator = 8+3+1 = 12, make it larger just in case
  byte buffer_idx = 0;

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
  lcd.setCursor(0, 1); // Go to the second line of the LCD
  lcd.print(uid_display_buffer); // Display the formatted UID

  Serial.println("Card Scanned!");
  lcd.setCursor(0, 0); // Back to first line after a delay
  lcd.print("Card Scanned!   "); // Overwrite "UID:"


  // Halt PICC
  mfrc522.PICC_HaltA();

  delay(2000); // Display UID for 2 seconds
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready to scan!"); // Reset LCD message
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear second line
}
