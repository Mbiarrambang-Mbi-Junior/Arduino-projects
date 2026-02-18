/* || @project Pursar Offline Vending System
|| @version 1.1 (Stable)
|| @author  Mbiarrambang Mbi Junior / Chadash Automation
|| @contact mbiarrambangmbijunior@gmail.com
||
|| @description:
|| Secure vending controller with Bluetooth confirmation, EEPROM 
|| price management, and authenticated admin modes.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

// --- Keypad Setup ---
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 9, 8, 7, 6 }; 
byte colPins[COLS] = { 5, 4, 3, 2 }; 

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// --- Hardware & Memory Mapping ---
const int buzzer = 10;
const int relayPin = 13;
const int PASS_ADDR = 500;       // Password storage (4 bytes)
const int CONFIG_FLAG_ADDR = 499; // Setup flag address

String currentAdminPass = ""; 
bool isAdmin = false; 
String inputCode = "";

struct Product {
  int price;
  byte stock;
};

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial BTSerial(11, 12); // RX (to BT TX), TX (to BT RX)

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  
  Serial.begin(9600);
  BTSerial.begin(9600); // Standard Baud for HC-05
  
  lcd.init();
  lcd.backlight();

  loadPassword(); // Secure EEPROM initialization
  showWelcome();
}

void loop() {
  char key = keypad.getKey();
  
  // Handle Keypad Input
  if (key) {
    tone(buzzer, 1000, 50);
    if (key == '#') {
      if (inputCode.length() > 0) {
        validateSelection(inputCode);
        inputCode = "";
      }
    } else if (key == '*') {
      inputCode = "";
      isAdmin = false; // Relock security on reset
      showWelcome();
    } else {
      inputCode += key;
      lcd.setCursor(0, 1);
      lcd.print("Code: " + inputCode + "   ");
    }
  }

  // Bluetooth Command Listener
  if (BTSerial.available()) {
    String res = BTSerial.readStringUntil('\n'); // Expects LF (\n)
    res.trim();
    
    if (res.startsWith("PASS:")) {
      checkAdmin(res);
    } else if (res.startsWith("PAID")) {
      handlePaymentSuccess();
    } else if (res.startsWith("SET:")) {
      if (isAdmin) remoteUpdate(res);
      else accessDenied();
    } else if (res.startsWith("NEWPASS:")) {
      if (isAdmin) changePassword(res);
      else accessDenied();
    }
  }
}

// --- CORE FUNCTIONS ---

void loadPassword() {
  byte flag = EEPROM.read(CONFIG_FLAG_ADDR);
  
  // If no flag found (factory state), set default 0000
  if (flag != 77) {
    saveNewPassword("0000");
    EEPROM.write(CONFIG_FLAG_ADDR, 77);
  } else {
    currentAdminPass = "";
    for (int i = 0; i < 4; i++) {
      currentAdminPass += (char)EEPROM.read(PASS_ADDR + i);
    }
  }
}

void saveNewPassword(String newP) {
  if (newP.length() == 4) {
    for (int i = 0; i < 4; i++) {
      EEPROM.update(PASS_ADDR + i, newP[i]); // update saves EEPROM life
    }
    currentAdminPass = newP;
  }
}

void checkAdmin(String data) {
  String providedPass = data.substring(5);
  if (providedPass == currentAdminPass) {
    isAdmin = true;
    lcd.clear();
    lcd.print("   ADMIN MODE   ");
    lcd.setCursor(0, 1);
    lcd.print("SET: or NEWPASS:");
    BTSerial.println("AUTH_SUCCESS: Admin Mode Active");
    tone(buzzer, 1500, 100); delay(100); tone(buzzer, 2000, 100);
  } else {
    BTSerial.println("AUTH_FAILED: Invalid Password");
    tone(buzzer, 200, 600);
  }
}

void remoteUpdate(String data) {
  int firstColon = data.indexOf(':');
  int secondColon = data.indexOf(':', firstColon + 1);
  if(firstColon != -1 && secondColon != -1) {
    int id = data.substring(firstColon + 1, secondColon).toInt();
    int price = data.substring(secondColon + 1).toInt();
    Product p = {price, 10}; 
    EEPROM.put(id * sizeof(Product), p); // Save to local database
    BTSerial.println("UPDATE_SUCCESS: Price Saved");
    lcd.clear(); lcd.print("Price Updated!");
    delay(2000);
    isAdmin = false; // Security relock
    showWelcome();
  }
}

void changePassword(String data) {
  String newPass = data.substring(8);
  if (newPass.length() == 4) {
    saveNewPassword(newPass);
    BTSerial.println("PASS_SUCCESS: New Password Active");
    lcd.clear(); lcd.print("PASS CHANGED!");
    delay(2000);
    isAdmin = false;
    showWelcome();
  }
}

void validateSelection(String code) {
  int id = code.toInt();
  if (id < 0 || id > 50) {
    lcd.clear(); lcd.print("Invalid Code");
    delay(2000); showWelcome();
    return;
  }
  
  Product p;
  EEPROM.get(id * sizeof(Product), p);
  if (p.price <= 0 || p.price > 10000) p.price = 500; // Default fallback

  lcd.clear();
  lcd.print(String(p.price) + " XAF");
  lcd.setCursor(0, 1);
  lcd.print("Confirm on App.."); 
  
  // Handshake to Bluetooth App
  BTSerial.print("REQ:"); BTSerial.print(id); BTSerial.print(":"); BTSerial.println(p.price);
}

void handlePaymentSuccess() {
  lcd.clear();
  lcd.print("Payment Verified");
  lcd.setCursor(0, 1);
  lcd.print("Dispensing...");
  tone(buzzer, 2000, 500); 
  digitalWrite(relayPin, HIGH); // Activate dispense relay
  delay(3000); 
  digitalWrite(relayPin, LOW);
  showWelcome();
}

void showWelcome() {
  lcd.clear();
  lcd.print("PURSAR OFFLINE");
  lcd.setCursor(0, 1);
  lcd.print("Enter Prod Code");
}

void accessDenied() {
  BTSerial.println("ACCESS_DENIED: Authenticate via PASS: first");
  tone(buzzer, 300, 500);
}