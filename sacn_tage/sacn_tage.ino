#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

// RFID MFRC522 pin definition
#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Student Data Structure
struct Student {
  byte uid[4];
  const char* name;
}; // <-- Semicolon added

// Available students
Student students[] = {
  // Example Student 1: UID {0x53, 0x9F, 0xAD, 0x0F} -> Name "Junior"
  {{0x53, 0x9F, 0xAD, 0x0F}, "Junior"},
  // Example Student 2: UID {0xB3, 0xE9, 0xF9, 0xF7} -> Name "Mbi"
  {{0xB3, 0xE9, 0xF9, 0xF7}, "Mbi"},
};

// LED Pin
int ledPin = 6;
// Calculate the total number of students
const int numStudents = sizeof(students) / sizeof(students[0]); // <-- Corrected calculation

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Place your RFID tag on the reader...");
  pinMode(ledPin, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.print("Initialising...");
  lcd.setCursor(0, 1);
  lcd.print("Please wait...");
  delay(2000);

  lcd.clear();
  lcd.print("WELCOME!");
  lcd.setCursor(0, 1);
  lcd.print("Scan your card");
  Serial.println("System Ready. Waiting for card scans...");
  digitalWrite(ledPin, HIGH); // <-- Turn LED ON at start
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  int studentIndex = -1;

  for (int i = 0; i < numStudents; i++) { // <-- Corrected loop condition
    if (memcmp(rfid.uid.uidByte, students[i].uid, 4) == 0) {
      studentIndex = i;
      break;
    }
  }

  lcd.clear();

  if (studentIndex != -1) { // <-- Corrected condition
    String studentName = students[studentIndex].name;
    Serial.print("Access granted for: ");
    Serial.println(studentName);

    lcd.setCursor(0, 0);
    lcd.print("Welcome,");
    lcd.setCursor(0, 1);
    lcd.print(studentName);
    digitalWrite(ledPin, HIGH); // <-- LED stays ON for granted access
    delay(3000);
  } else {
    Serial.println("Access Denied: Unknown card!");
    lcd.setCursor(0, 0);
    lcd.print("Access Denied!");
    digitalWrite(ledPin, LOW); // <-- Turn LED OFF for denied access
    lcd.setCursor(0, 1);
    lcd.print("unknown card 💳");
    delay(3000);
  }
  
  // Return to the initial state after a delay
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME!");
  lcd.setCursor(0, 1);
  lcd.print("Scan your card");

  Serial.print("UID tag:");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(" ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();

  rfid.PICC_HaltA();
}