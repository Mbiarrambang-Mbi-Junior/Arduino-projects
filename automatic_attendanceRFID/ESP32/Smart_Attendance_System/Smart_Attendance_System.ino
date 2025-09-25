#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>          // For ESP32 Wi-Fi connectivity
#include <HTTPClient.h>    // For making HTTP requests
#include "time.h"          // For NTP time synchronization

// --- Wi-Fi Credentials ---
const char* ssid = "YOUR_WIFI_SSID";         // REPLACE WITH YOUR WIFI SSID
const char* password = "YOUR_WIFI_PASSWORD"; // REPLACE WITH YOUR WIFI PASSWORD

// --- Google Apps Script Web App URL ---
// REPLACE THIS WITH THE URL YOU GET AFTER DEPLOYING YOUR GOOGLE APPS SCRIPT
const char* googleSheetsScriptURL = "YOUR_GOOGLE_APPS_SCRIPT_WEB_APP_URL_HERE";

// --- Time Zone Information for NTP ---
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600; // GMT+1 for Cameroon (3600 seconds)
const int   daylightOffset_sec = 0; // No daylight saving for Cameroon

// --- RFID MFRC522 pin definitions ---
// These are common SPI pins for ESP32. Adjust if your wiring is different.
#define SS_PIN 5              // ESP32 D5 (GPIO5) - SDA (SS) for MFRC522
#define RST_PIN 27            // ESP32 D27 (GPIO27) - RST for MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance for RFID reader

// --- Buzzer & LED pin definitions ---
const int buzzerPin = 18;  // Example ESP32 GPIO pin
const int yellowLED = 19;  // Example ESP32 GPIO pin
const int greenLED = 23;   // Example ESP32 GPIO pin
const int redLED = 22;     // Example ESP32 GPIO pin

// --- LCD I2C definitions ---
// SDA and SCL are typically GPIO21 and GPIO22 on ESP32, but Wire.begin() handles this.
// Ensure your LCD I2C address is correct.
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address and dimensions (16 columns, 2 rows)

// --- Student Data Structure ---
struct Student {
  byte uid[4];       // Stores the 4-byte Unique Identifier (UID) of the RFID card
  const char* name;  // Stores a pointer to the student's name string
};

// --- Authorized Student UIDs and Names ---
/* YOU MUST REPLACE THESE UIDS WITH THE ACTUAL UIDS OF YOUR RFID CARDS!
   To find your card UIDs:
   1. Upload your original basic RFID sketch (View_TagsUID.ino) or use a modified version
      to print UID to Serial Monitor on ESP32.
   2. Open the Serial Monitor.
   3. Scan each RFID card and note down the "Card UID:" printed in hexadecimal format (e.g., DD 99 CB 7A).
   4. Convert those hex values into the format {0xDD, 0x99, 0xCB, 0x7A} for your cards.
*/
Student students[] = {
  // Example Student 1: UID {0xDD, 0x99, 0xCB, 0x7A} -> Name "Alice"
  {{0xDD, 0x99, 0xCB, 0x7A}, "Alice"},
  // Example Student 3: UID {0xAA, 0xBB, 0xCC, 0xDD} -> Name "Charlie"},
  {{0xAA, 0xBB, 0xCC, 0xDD}, "Charlie"},
  // You can add more students following the same format
  // {{0xXX, 0xXX, 0xXX, 0xXX}, "Student Name"}
};
// Calculate the number of students in the array automatically
const int numStudents = sizeof(students) / sizeof(students[0]);

void setup() {
  Serial.begin(115200); // Higher baud rate for ESP32

  // --- Configure Pin Modes ---
  pinMode(buzzerPin, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  digitalWrite(yellowLED, HIGH); // Turn yellow LED ON to indicate system is powered/initializing

  // --- Initialize LCD ---
  lcd.init();
  lcd.backlight();
  lcd.print("Initializing...");
  lcd.setCursor(0, 1);
  lcd.print("Please wait...");

  // --- Initialize SPI bus for MFRC522 ---
  SPI.begin();
  mfrc522.PCD_Init();

  // --- Connect to Wi-Fi ---
  Serial.print("Connecting to WiFi ");
  Serial.println(ssid);
  lcd.clear();
  lcd.print("Connecting WiFi...");
  lcd.setCursor(0, 1);
  lcd.print(ssid);

  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Try for 20 seconds
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    lcd.clear();
    lcd.print("WiFi Connected!");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    delay(2000);

    // --- Initialize NTP Time ---
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime(); // Function to print time (for debugging)
    Serial.println("Time synchronized via NTP.");

  } else {
    Serial.println("\nFailed to connect to WiFi. Halting.");
    lcd.clear();
    lcd.print("WiFi Failed!");
    lcd.setCursor(0, 1);
    lcd.print("Check credentials.");
    digitalWrite(redLED, HIGH); // Indicate error
    while (true) { delay(100); } // Halt program
  }

  // --- System Ready Message ---
  lcd.clear();
  lcd.print("WELCOME!");
  lcd.setCursor(0, 1);
  lcd.print("Scan your card");
  digitalWrite(yellowLED, HIGH); // Yellow LED remains ON to show system is active
  Serial.println("System Ready. Waiting for card scans...");
}

void loop() {
  // Check Wi-Fi connection periodically
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    lcd.clear();
    lcd.print("WiFi Discnnected");
    lcd.setCursor(0, 1);
    lcd.print("Reconnecting...");
    digitalWrite(redLED, HIGH); // Indicate network issue
    WiFi.reconnect();
    delay(5000); // Wait 5 seconds before trying again
    return; // Skip current loop iteration if no Wi-Fi
  } else {
    digitalWrite(redLED, LOW); // Turn off red LED if reconnected
  }

  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // --- Get current date and time from NTP synchronized ESP32 clock ---
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time. NTP sync issue?");
    lcd.clear();
    lcd.print("Time Error!");
    lcd.setCursor(0, 1);
    lcd.print("Reboot ESP32.");
    delay(2000);
    return; // Skip logging if time isn't available
  }
  char timeStr[25]; // Buffer for formatted timestamp
  strftime(timeStr, sizeof(timeStr), "%d/%m/%Y %H:%M:%S", &timeinfo);

  // --- Format and print the scanned Card UID to Serial Monitor ---
  Serial.print("Scanned Card UID: ");
  char currentUID_hex_str[12]; // Buffer to store UID in "XX XX XX XX" format
  sprintf(currentUID_hex_str, "%02X %02X %02X %02X",
    mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1],
    mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3]);
  Serial.println(currentUID_hex_str);

  // --- Student Identification Logic ---
  int studentIndex = -1;
  for (int i = 0; i < numStudents; i++) {
    if (memcmp(mfrc522.uid.uidByte, students[i].uid, 4) == 0) {
      studentIndex = i;
      break;
    }
  }

  lcd.clear();
  String studentName = "";
  String status = "";

  if (studentIndex != -1) {
    studentName = students[studentIndex].name;
    status = "IN"; // For a simple check-in/out, we'll just log "IN" for now
    Serial.print("Access Granted for: ");
    Serial.println(studentName);
    lcd.setCursor(0, 0);
    lcd.print("Welcome,");
    lcd.setCursor(0, 1);
    lcd.print(studentName);
    playSuccessSoundAndLight();
  } else {
    studentName = "UNKNOWN";
    status = "DENIED";
    Serial.println("Access Denied: Unknown Card!");
    lcd.setCursor(0, 0);
    lcd.print("Access Denied!");
    lcd.setCursor(0, 1);
    lcd.print("Unknown Card.");
    playDeniedSoundAndLight();
  }

  Serial.print("Timestamp: ");
  Serial.println(timeStr);
  Serial.print("Status: ");
  Serial.println(status);

  // --- Send Data to Google Sheets ---
  String postData = "uid=" + String(currentUID_hex_str) +
                    "&name=" + studentName +
                    "&timestamp=" + String(timeStr) +
                    "&status=" + status;

  Serial.println("Sending data to Google Sheets...");
  sendToGoogleSheets(postData);

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(3000); // Keep message on LCD for 3 seconds
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your card");
  lcd.setCursor(0, 1);
  lcd.print("          ");
  digitalWrite(yellowLED, HIGH); // Ensure yellow LED is back on
}

// --- Helper Functions ---

void sendToGoogleSheets(String postData) {
  HTTPClient http;
  http.begin(googleSheetsScriptURL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    String response = http.getString();
    Serial.println("Server Response: " + response);
    lcd.clear();
    lcd.print("Data Sent!");
    lcd.setCursor(0, 1);
    lcd.print("Status: OK");
  } else {
    Serial.printf("HTTP Error: %s\n", http.errorToString(httpResponseCode).c_str());
    lcd.clear();
    lcd.print("Sheet Send Error");
    lcd.setCursor(0, 1);
    lcd.print("Code: " + String(httpResponseCode));
  }
  http.end(); // Free resources
  delay(1000); // Short delay to display send status
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void playSuccessSoundAndLight() {
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(greenLED, HIGH);
  delay(200);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(greenLED, LOW);
  delay(100);
  digitalWrite(greenLED, HIGH);
  delay(100);
  digitalWrite(greenLED, LOW);
}

void playDeniedSoundAndLight() {
  digitalWrite(yellowLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(redLED, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(redLED, LOW);
  delay(100);
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(redLED, HIGH);
  delay(100);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(redLED, LOW);
}