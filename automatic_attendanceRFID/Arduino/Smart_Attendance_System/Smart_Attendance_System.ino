#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>              // Required for I2C communication
#include <LiquidCrystal_I2C.h> // Required for LCD I2C
#include <RTClib.h>            // Required for Real-Time Clock (RTC) module
#include <SD.h>                // Required for SD card module

// --- RFID MFRC522 pin definitions ---
#define SS_PIN 10              // Configurable pin for MFRC522 Slave Select (SS)
#define RST_PIN 9              // Configurable pin for MFRC522 Reset (RST)
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance for RFID reader

// --- Buzzer & LED pin definitions ---
const int buzzerPin = 5;
const int yellowLED = 3;
const int greenLED = 6;
const int redLED = 7;

// --- LCD I2C definitions ---
/*  Replace 0x27 with the actual I2C address of your LCD.
 You can find this address by running an I2C scanner sketch.
 Common addresses are 0x27 or 0x3F.
 For me it's 16x2 LCD: LiquidCrystal_I2C(address, 16, 2)
 since the RTC time module will be using the SDA and SDL pin on the arduino 
 they will both be sharing the pins
*/
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address and dimensions (16 columns, 2 rows)

// --- RTC Module ---
// Choose the correct RTC object based on your module:
RTC_DS3231 rtc; // For DS3231 module (recommended for accuracy) and what i actualy have
// RTC_DS1307 rtc; // For DS1307 module

/* --- SD Card Module ---
 Define the Chip Select (CS) pin for your SD card module.
 This is typically D4 or D10, but check your specific module's documentation.
 also the SD card will use the same pins as the MFRC522 Pins
 this is becuase the arduino  will first send that device's address on the SDA line. Only the device that recognizes its address will respond; 
 all other devices on the bus ignore the communication.
*/
const int chipSelect = 4; // Digital pin connected to SD card CS

// --- Student Data Structure ---
// Defines a structure to hold student information.
struct Student {
  byte uid[4];       // Stores the 4-byte Unique Identifier (UID) of the RFID card
  const char* name;  // Stores a pointer to the student's name string
};

// --- Authorized Student UIDs and Names ---
/* YOU MUST REPLACE THESE UIDS WITH THE ACTUAL UIDS OF YOUR RFID CARDS!
 To find your card UIDs:
 1. Upload your original basic RFID sketch (View_TagsUID.ino).
 2. Open the Serial Monitor.
 3. Scan each RFID card and note down the "Card UID:" printed in hexadecimal format (e.g., DD 99 CB 7A).
 4. Convert those hex values into the format {0xDD, 0x99, 0xCB, 0x7A} for your cards.
*/
Student students[] = {
  // Example Student 1: UID {0xDD, 0x99, 0xCB, 0x7A} -> Name "Alice"
  {{0xDD, 0x99, 0xCB, 0x7A}, "Alice"},
  // Example Student 3: UID {0xAA, 0xBB, 0xCC, 0xDD} -> Name "Charlie"
  {{0xAA, 0xBB, 0xCC, 0xDD}, "Charlie"},
  // You can add more students following the same format
  // {{0xXX, 0xXX, 0xXX, 0xXX}, "Student Name"}
};
// Calculate the number of students in the array automatically
const int numStudents = sizeof(students) / sizeof(students[0]);

// --- File Name for Attendance Logging on SD Card ---
const char* attendanceLogFile = "ATTEND.CSV"; // Using .CSV for easier spreadsheet compatibility

void setup() {
  // --- Configure Pin Modes ---
  pinMode(buzzerPin, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT); // Set red LED pin as output

  Serial.begin(9600); // Start serial communication for debugging output to PC

  SPI.begin(); // Initialize SPI bus for MFRC522 and SD card communication

  mfrc522.PCD_Init(); // Initialize the RFID reader module

  lcd.init();      // Initialize the LCD module
  lcd.backlight(); // Turn on the LCD backlight (if available)

  // Initial welcome message on LCD
  lcd.print("Initializing...");
  lcd.setCursor(0, 1); // Set cursor to the second line, first column
  lcd.print("Please wait...");
  digitalWrite(yellowLED, HIGH); // Turn yellow LED ON to indicate system is powered/initializing

  // --- Initialize RTC Module ---
  // Check if the RTC module is connected and functioning.
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC module!");
    lcd.clear();
    lcd.print("RTC Error!");
    lcd.setCursor(0, 1);
    lcd.print("Check Wiring.");
    while (1); // Halt the program if RTC is not found (critical component)
  }

  // OPTIONAL: Uncomment the line below *once* to set the RTC to the Arduino's compile time.
  // After the first successful upload and time set, COMMENT THIS LINE OUT AGAIN,
  // otherwise, the RTC will reset to compile time every time you upload the sketch.

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); /*this is the line */

  // If the RTC is not running, set it to the compile time (useful if battery is dead)
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, setting time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // --- Initialize SD Card Module ---
  Serial.print("Initializing SD card...");
  // Check if the SD card is present and can be initialized.
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card failed, or not present");
    lcd.clear();
    lcd.print("SD Card Error!");
    lcd.setCursor(0, 1);
    lcd.print("Check SD Card.");
    while (1); // Halt the program if SD card is not found (critical for logging)
  }
  Serial.println("SD card initialized successfully.");

  // Check if the log file exists. If not, create it with a header.
  if (!SD.exists(attendanceLogFile)) {
    File dataFile = SD.open(attendanceLogFile, FILE_WRITE);
    if (dataFile) {
      dataFile.println("UID,Name,Timestamp,Status"); // CSV Header
      dataFile.close();
      Serial.println("Created new attendance log file with header.");
    } else {
      Serial.println("Error creating attendance log file!");
    }
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
  // Check if a new RFID card is present near the reader
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return; // No new card, exit loop iteration
  }

  // Read the selected card's UID
  if (!mfrc522.PICC_ReadCardSerial()) {
    return; // Failed to read card serial, exit loop iteration
  }

  // --- Get current date and time from RTC ---
  DateTime now = rtc.now(); // Get the current date and time
  char timeStr[20]; // Buffer to store the formatted timestamp string
  // Format the timestamp: DD/MM/YYYY HH:MM:SS
  sprintf(timeStr, "%02d/%02d/%04d %02d:%02d:%02d",
    now.day(), now.month(), now.year(),
    now.hour(), now.minute(), now.second());

  // --- Format and print the scanned Card UID to Serial Monitor ---
  Serial.print("Scanned Card UID: ");
  char currentUID_hex_str[12]; // Buffer to store UID in "XX XX XX XX" format
  sprintf(currentUID_hex_str, "%02X %02X %02X %02X",
    mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1],
    mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3]);
  Serial.println(currentUID_hex_str); // Print to Serial Monitor

  // --- Student Identification Logic ---
  int studentIndex = -1; // Initialize to -1 (means student not found)
  // Loop through the array of authorized students to find a match
  for (int i = 0; i < numStudents; i++) {
    // memcmp compares memory blocks (UID bytes)
    if (memcmp(mfrc522.uid.uidByte, students[i].uid, 4) == 0) {
      studentIndex = i; // Found a match, store the index
      break;            // Exit the loop
    }
  }

  lcd.clear(); // Clear the LCD for new message
  String logEntry; // String to build the attendance record for the SD card

  if (studentIndex != -1) {
    // --- Student Found: Grant Access and Log Check-In/Check-Out ---
    String studentName = students[studentIndex].name;
    Serial.print("Access Granted for: ");
    Serial.println(studentName);
    Serial.print("Timestamp: ");
    Serial.println(timeStr);

    lcd.setCursor(0, 0);
    lcd.print("Welcome,");
    lcd.setCursor(0, 1);
    lcd.print(studentName); // Display student's name

    // Placeholder for simple IN/OUT logic (can be expanded)
    // For a more advanced system, you'd track the last state (IN/OUT)
    // For this example, we'll just log "IN" for recognized students.
    //And when i will advance or iterate the project i will check for in and out
    logEntry = String(currentUID_hex_str) + "," + studentName + "," + String(timeStr) + ",IN";
    playSuccessSoundAndLight(); // Trigger success feedback

  } else {
    // --- Unknown Card: Deny Access ---
    Serial.println("Access Denied: Unknown Card!");
    Serial.print("Timestamp: ");
    Serial.println(timeStr);

    lcd.setCursor(0, 0);
    lcd.print("Access Denied!");
    lcd.setCursor(0, 1);
    lcd.print("Unknown Card.");

    // Log entry for unknown cards
    logEntry = String(currentUID_hex_str) + ",UNKNOWN," + String(timeStr) + ",DENIED";
    playDeniedSoundAndLight(); // Trigger denial feedback
  }

  // --- Log Attendance to SD Card ---
  File dataFile = SD.open(attendanceLogFile, FILE_WRITE); // Open the file in write mode (appends to end)
  if (dataFile) {
    dataFile.println(logEntry); // Write the attendance record line
    dataFile.close();           // Close the file to save data
    Serial.println("Logged to SD Card: " + logEntry);
  } else {
    // If there's an error opening the SD card file
    Serial.println("Error opening " + String(attendanceLogFile) + " for writing.");
    lcd.clear();
    lcd.print("SD Write Error!");
    lcd.setCursor(0, 1);
    lcd.print("Check SD Card.");
    delay(1000); // Display error for a moment
  }

  // --- Reset RFID Reader for Next Scan ---
  mfrc522.PICC_HaltA();       // Halt PICC (effectively deactivates the card)
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD (important for next scan)

  delay(3000); // Keep the result message on LCD for 3 seconds
  // --- Prepare LCD for next scan ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your card"); // Reset LCD message
  lcd.setCursor(0, 1);
  lcd.print("          "); // Clear second line to remove previous UID/name
  digitalWrite(yellowLED, HIGH); // Ensure yellow LED is back on after feedback
}

// --- Helper Functions for Audio and Visual Feedback ---

void playSuccessSoundAndLight() {
  digitalWrite(yellowLED, LOW); // Turn yellow OFF temporarily
  digitalWrite(redLED, LOW);    // Ensure red is OFF
  digitalWrite(buzzerPin, HIGH); // Buzzer ON
  digitalWrite(greenLED, HIGH);  // Green LED ON
  delay(200);                    // Stay on for 200ms
  digitalWrite(buzzerPin, LOW);  // Buzzer OFF
  digitalWrite(greenLED, LOW);   // Green LED OFF
  delay(100);                    // Short pause
  digitalWrite(greenLED, HIGH);  // Green LED flash
  delay(100);
  digitalWrite(greenLED, LOW);   // Green LED OFF
}

void playDeniedSoundAndLight() {
  digitalWrite(yellowLED, LOW); // Turn yellow OFF temporarily
  digitalWrite(greenLED, LOW);  // Ensure green is OFF
  digitalWrite(buzzerPin, HIGH); // Buzzer ON (first beep)
  digitalWrite(redLED, HIGH);    // Red LED ON
  delay(100);
  digitalWrite(buzzerPin, LOW);  // Buzzer OFF
  digitalWrite(redLED, LOW);     // Red LED OFF
  delay(100);
  digitalWrite(buzzerPin, HIGH); // Buzzer ON (second beep)
  digitalWrite(redLED, HIGH);    // Red LED ON
  delay(100);
  digitalWrite(buzzerPin, LOW);  // Buzzer OFF
  digitalWrite(redLED, LOW);     // Red LED OFF
}
