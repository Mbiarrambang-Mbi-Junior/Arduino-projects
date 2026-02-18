/* @file Bluetooth-vend
|| @version 1.1 (Stable)
|| @author Mbiarrambang Mbi junior
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

// --- Hardware Pins ---
int buzzer = 10;
int relayPin = 13;
const String BT_PIN = "1234"; 

// --- Database Structure ---
struct Product {
  int price;
  byte stock;
};

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial BTSerial(11, 12); // RX, TX
String inputCode = "";

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  
  Serial.println("--- PURSAR SYSTEM ACTIVE ---");
  showWelcome(); // Critical: Show welcome message on boot
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Key Pressed: "); Serial.println(key);
    tone(buzzer, 1000, 50);

    if (key == '#') {
      if (inputCode.length() > 0) {
        validateSelection(inputCode);
        inputCode = "";
      }
    } else if (key == '*') {
      inputCode = "";
      showWelcome();
    } else {
      // Append key to code
      inputCode += key;
      lcd.setCursor(0, 1);
      lcd.print("Code: " + inputCode + "   ");
    }
  }

  // Bluetooth Listener
  if (BTSerial.available()) {
    String res = BTSerial.readStringUntil('\n');
    res.trim();
    
    if (res.startsWith("PAID")) {
       handlePaymentSuccess();
    } else if (res.startsWith("SET:")) {
       remoteUpdate(res);
    }
  }
}

void showWelcome() {
  lcd.clear();
  lcd.print("PURSAR OFFLINE");
  lcd.setCursor(0, 1);
  lcd.print("Enter Prod Code");
}

void validateSelection(String code) {
  int id = code.toInt();
  
  // Security check: limit ID to a reasonable range (e.g., 0-50)
  if (id < 0 || id > 50) {
    lcd.clear();
    lcd.print("Invalid Code");
    delay(2000);
    showWelcome();
    return;
  }

  Product p;
  EEPROM.get(id * sizeof(Product), p);

  // If memory is empty (contains -1 or 0), set default 500
  if (p.price <= 0 || p.price > 10000) p.price = 500;

  lcd.clear();
  lcd.print(String(p.price) + " XAF");
  lcd.setCursor(0, 1);
  lcd.print("Confirm on App.."); 
  
  // Handshake to Phone
  BTSerial.print("REQ:"); 
  BTSerial.print(id); 
  BTSerial.print(":"); 
  BTSerial.println(p.price);
  
  Serial.print("Request Sent for ID: "); Serial.println(id);
}

void handlePaymentSuccess() {
  lcd.clear();
  lcd.print("Payment Verified");
  lcd.setCursor(0, 1);
  lcd.print("Dispensing...");
  
  tone(buzzer, 2000, 500); 
  digitalWrite(relayPin, HIGH);
  delay(3000); 
  digitalWrite(relayPin, LOW);
  
  showWelcome();
}

void remoteUpdate(String data) {
  int firstColon = data.indexOf(':');
  int secondColon = data.indexOf(':', firstColon + 1);
  
  if(firstColon != -1 && secondColon != -1) {
    int id = data.substring(firstColon + 1, secondColon).toInt();
    int price = data.substring(secondColon + 1).toInt();
    
    Product p = {price, 10}; // Default stock to 10
    EEPROM.put(id * sizeof(Product), p);
    
    BTSerial.println("Price Updated!");
    // Success Beeps
    tone(buzzer, 1500, 100); delay(100); tone(buzzer, 2000, 100);
  }
}