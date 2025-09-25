#include <Wire.h>          // Required for I2C communication
#include <LiquidCrystal_I2C.h> // Required for LCD I2C
#include <Keypad.h>        // Required for matrix keypad

// --- LCD I2C Configuration ---
// Set the LCD I2C address and dimensions (e.g., 16 columns, 2 rows)
// Verify your LCD I2C address using the I2C scanner sketch if 0x27 doesn't work.
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- Keypad Configuration ---
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Define the keypad layout (char array representing the buttons)
char keys[ROWS][COLS] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'.','0','=','/'}
};

// Define the Arduino pins connected to the keypad rows
byte rowPins[ROWS] = {9, 8, 7, 6}; // Connect to D9, D8, D7, D6

// Define the Arduino pins connected to the keypad columns
byte colPins[COLS] = {5, 4, 3, 2}; // Connect to D5, D4, D3, D2

// Create the Keypad object
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- Calculator Variables ---
String currentInput = ""; // Stores the number currently being entered
double firstNumber = 0;
double secondNumber = 0;
char operation = ' '; // Stores the operation (+, -, *, /)
bool resultDisplayed = false; // Flag to know if a result is on screen

void setup() {
  Serial.begin(9600); // For debugging output to Serial Monitor

  // Initialize the LCD
  lcd.init();
  lcd.backlight(); // Turn on the backlight
  lcd.clear();
  lcd.print("Arduino Calc");
  lcd.setCursor(0, 1);
  lcd.print("Enter numbers...");
}

void loop() {
  char customKey = customKeypad.getKey(); // Read a key press

  if (customKey) { // If a key was pressed
    if (resultDisplayed && (customKey >= '0' && customKey <= '9' || customKey == '.')) {
      // If a new number is started after a result, clear everything
      clearCalculator();
    }
    resultDisplayed = false; // Reset result flag on any new input

    switch (customKey) {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
      case '.':
        currentInput += customKey; // Append digit/decimal to current number string
        lcd.setCursor(0, 0);
        lcd.print("                "); // Clear first line
        lcd.setCursor(0, 0);
        lcd.print(currentInput); // Display current input
        break;

      case '+': case '-': case '*': case '/':
        if (currentInput.length() > 0) {
          firstNumber = currentInput.toDouble(); // Convert current input to a number
          operation = customKey; // Store the operation
          currentInput = ""; // Clear input for the next number
          lcd.setCursor(0, 1);
          lcd.print("                "); // Clear second line
          lcd.setCursor(0, 1);
          lcd.print(firstNumber);
          lcd.print(operation); // Display first number and operation
        }
        break;

      case '=':
        if (currentInput.length() > 0 && operation != ' ') {
          secondNumber = currentInput.toDouble(); // Get the second number
          double result = calculate(firstNumber, secondNumber, operation); // Perform calculation

          lcd.clear();
          lcd.print(firstNumber, 3); // Print first number (3 decimal places)
          lcd.print(operation);
          lcd.print(secondNumber, 3); // Print second number
          lcd.setCursor(0, 1);
          lcd.print("=");
          lcd.print(result, 3); // Print result (3 decimal places)

          currentInput = String(result); // Set result as new currentInput for chained calculations
          operation = ' '; // Clear operation
          firstNumber = result; // Update first number for chaining
          resultDisplayed = true;
        } else if (currentInput.length() > 0 && operation == ' ') {
          // If '=' pressed with only one number, just display it on line 1 and clear line 2
          lcd.setCursor(0,0);
          lcd.print("                ");
          lcd.setCursor(0,0);
          lcd.print(currentInput);
          lcd.setCursor(0,1);
          lcd.print("                ");
          firstNumber = currentInput.toDouble(); // For chaining
        }
        break;

      case '#': // Using '#' for clear/reset
        clearCalculator();
        break;
    }
    // Debug to Serial Monitor
    Serial.print("Key: "); Serial.println(customKey);
    Serial.print("Current Input: "); Serial.println(currentInput);
    Serial.print("First Number: "); Serial.println(firstNumber);
    Serial.print("Operation: "); Serial.println(operation);
  }
}

// Function to perform the arithmetic calculation
double calculate(double num1, double num2, char op) {
  switch (op) {
    case '+':
      return num1 + num2;
    case '-':
      return num1 - num2;
    case '*':
      return num1 * num2;
    case '/':
      if (num2 != 0) { // Prevent division by zero
        return num1 / num2;
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Div by Zero Err!");
        delay(1500);
        return 0; // Return 0 or handle error appropriately
      }
    default:
      return 0; // Should not happen
  }
}

// Function to reset the calculator state
void clearCalculator() {
  currentInput = "";
  firstNumber = 0;
  secondNumber = 0;
  operation = ' ';
  resultDisplayed = false;
  lcd.clear();
  lcd.print("Cleared!");
  lcd.setCursor(0, 1);
  lcd.print("Enter numbers...");
  delay(1000); // Small delay to show "Cleared!"
  lcd.clear();
}