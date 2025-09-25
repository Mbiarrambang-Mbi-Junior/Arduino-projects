// firebase libraire's
#include <FB_Const.h>
#include <FB_Error.h>
#include <FB_Network.h>
#include <FB_Utils.h>
#include <Firebase.h>
#include <FirebaseFS.h>
#include <Firebase_ESP_Client.h>

// libraire's for rfid
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
// RFID MFRC522 pin definition
#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// your wifi credentials
#define WIFI_SSID "my_wifi_name"
#define WIFI_PASSWORD "my_wifi_password"

// your firebase credentials
#define FIREBASE_HOST "YOUR_DATABASE_URL"
#define FIREBASE_AUTH "YOUR_DATABASE_SECRET_KEY"

FirebaseData fbdo;

// any code for set up like pin defination// Student Data Structure
struct Student {
  byte uid[4];
  const char* name;
};
// Available students
Student students[] = {
  // Example Student 1: UID {0x53, 0x9F, 0xAD, 0x0F} -> Name "Junior"
  { { 0x53, 0x9F, 0xAD, 0x0F }, "Junior" },
  // Example Student 2: UID {0xB3, 0xE9, 0xF9, 0xF7} -> Name "Mbi"
  { { 0xB3, 0xE9, 0xF9, 0xF7 }, "Mbi" },
};

// LED Pin
int ledPin = 6;
// Calculate the total number of students
const int numStudents = sizeof(students) / sizeof(students[0]);

void setup() {
  // rfid setup
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Place your RFID tag on the reader...");
  pinMode(ledPin, OUTPUT);

  // wifi setup
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print();
  Serial.print("Connected With IP: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  // lcd setup
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
  digitalWrite(ledPin, Low);
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  int studentIndex = -1;

  for (int i = 0; i < numStudents; i++) { 
    if (memcmp(rfid.uid.uidByte, students[i].uid, 4) == 0) {
      studentIndex = i;
      break;
    }
  }

  lcd.clear();

  if (studentIndex != -1) {
    // ... (Access granted code)
    String studentName = students[studentIndex].name;
    Serial.print("Access granted for: ");
    Serial.println(studentName);

    lcd.setCursor(0, 0);
    lcd.print("Welcome,");
    lcd.setCursor(0, 1);
    lcd.print(studentName);
    digitalWrite(ledPin, HIGH); // <-- LED stays ON for granted access
    delay(3000);

    // Log the access to Firestore
    String logPath = "/access_logs/" + String(students[studentIndex].name);
    Firebase.setString(fbdo, logPath, "Access Granted");
  } else {
    // ... (Access denied code)
    Serial.println("Access Denied: Unknown card!");
    lcd.setCursor(0, 0);
    lcd.print("Access Denied!");
    digitalWrite(ledPin, LOW); // <-- Turn LED OFF for denied access
    lcd.setCursor(0, 1);
    lcd.print("unknown card 💳");
    delay(3000);

    // Log the denial to Firestore
    String logPath = "/access_logs/unknown_card";
    Firebase.setString(fbdo, logPath, "Access Denied");
  }

  // ... (existing print UID and halt PICC code)
}
