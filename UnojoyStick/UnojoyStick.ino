#include "UnoJoy.h"

// Define the pins for your joystick and buttons
const int JOYSTICK_X_PIN = A0; // Analog pin for Joystick X-axis
const int JOYSTICK_Y_PIN = A1; // Analog pin for Joystick Y-axis

// Define digital pins for your 4 buttons
// You can adjust these based on available pins and your wiring preference
const int BUTTON_1_PIN = 2; // Digital pin for Button 1 (e.g., Cross/A)
const int BUTTON_2_PIN = 3; // Digital pin for Button 2 (e.g., Circle/B)
const int BUTTON_3_PIN = 4; // Digital pin for Button 3 (e.g., Square/X)
const int BUTTON_4_PIN = 5; // Digital pin for Button 4 (e.g., Triangle/Y)

void setup() {
  setupPins();
  setupUnoJoy();
}

void loop() {
  // Always be getting fresh data
  dataForController_t controllerData = getControllerData();
  setControllerData(controllerData);
}

void setupPins(void) {
  // Set the button pins as inputs with pull-up resistors
  pinMode(BUTTON_1_PIN, INPUT_PULLUP); // INPUT_PULLUP activates internal pull-up resistor
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);
  pinMode(BUTTON_4_PIN, INPUT_PULLUP);

  // Joystick analog pins do not need pinMode configuration as they are implicitly inputs
  // when used with analogRead(). They also don't use pull-ups in the same way.
}

dataForController_t getControllerData(void) {
  // Set up a place for our controller data
  dataForController_t controllerData = getBlankDataForController();

  // Read button states
  // Buttons are active LOW when using INPUT_PULLUP (pressed connects to GND)
  // so we use '!' to invert the reading: LOW (pressed) becomes true
  controllerData.crossOn = !digitalRead(BUTTON_1_PIN);
  controllerData.circleOn = !digitalRead(BUTTON_2_PIN);
  controllerData.squareOn = !digitalRead(BUTTON_3_PIN);
  controllerData.triangleOn = !digitalRead(BUTTON_4_PIN);

  // --- Leave other buttons OFF by default ---
  // If you don't connect a button to a specific input,
  // UnoJoy defaults them to OFF when using getBlankDataForController().
  // We explicitly set them to false here to be clear, though it's
  // largely redundant if you use getBlankDataForController() first.
  controllerData.dpadUpOn = false;
  controllerData.dpadDownOn = false;
  controllerData.dpadLeftOn = false;
  controllerData.dpadRightOn = false;
  controllerData.l1On = false;
  controllerData.r1On = false;
  controllerData.l2On = false; // UnoJoy also has L2/R2, often used for triggers
  controllerData.r2On = false;
  controllerData.selectOn = false;
  controllerData.startOn = false;
  controllerData.homeOn = false;
  controllerData.l3On = false; // L3/R3 are usually joystick press buttons
  controllerData.r3On = false;


  // Set the analog sticks
  // UnoJoy expects 8-bit values (0-255) for stick positions.
  // analogRead() returns a 10-bit value (0-1023).
  // We bit-shift right by 2 (>> 2) to divide by 4, converting 1024 values to 256.
  controllerData.leftStickX = analogRead(JOYSTICK_X_PIN) >> 2;
  controllerData.leftStickY = analogRead(JOYSTICK_Y_PIN) >> 2;

  // Since you only want one joystick, set the right stick values to center
  // (or any default value that doesn't interfere)
  controllerData.rightStickX = 127; // Center value for 8-bit stick (0-255 range)
  controllerData.rightStickY = 127; // Center value for 8-bit stick (0-255 range)

  // And return the data!
  return controllerData;
}