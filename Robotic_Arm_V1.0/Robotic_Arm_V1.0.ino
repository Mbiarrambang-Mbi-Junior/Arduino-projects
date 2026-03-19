/*
  Controlling robotic arms with 3 servo motors and 1 stepper motor.
  Controlled via 4 potentiometers with Serial Monitor feedback.

  by Mbiarrambang Mbi Junior
  git: <http://people.interaction-ivrea.it/m.rinott>

  By chadash automation
  Version: 1.0
  course: http://www.chadash.io/en/Tutorial/robotic-arm
*/

// Libraries to control the stepper motor and servos
#include <Servo.h>
#include <Stepper.h>

// Pin Definitions (The "Where")
int basePot = A0;      // Controls the 28BYJ-48 Stepper Motor
int shoulderPot = A1;  // Controls the 1st 9g Servo (Shoulder)
int elbowPot = A2;     // Controls the 2nd 9g Servo (Elbow)
int wristPot = A3;     // Controls the 3rd 9g Servo (Wrist)

// Storage Variables (The "What")
int baseVal;      // Stores the raw analog reading for the Stepper
int shoulderVal;  // Stores the raw analog reading for the Shoulder Servo
int elbowVal;     // Stores the raw analog reading for the Elbow Servo
int wristVal;     // Stores the raw analog reading for the Wrist Servo

// Variables to track the previous position (needed for Serial Monitor logic)
int lastShoulder = 0;
int lastElbow = 0;
int lastWrist = 0;

// Servo object instances
Servo shoulderServo;  // Instance for the 1st 9g Servo (Shoulder)
Servo elbowServo;     // Instance for the 2nd 9g Servo (Elbow)
Servo wristServo;     // Instance for the 3rd 9g Servo (Wrist)

// Stepper Configuration
const int stepsPerRev = 2048;                  // Total steps for one full rotation of 28BYJ-48
Stepper baseStepper(stepsPerRev, 2, 4, 3, 5);  // Pins IN1, IN3, IN2, IN4
int currentStepperPos = 0;                     // Tracks the current position of the base

void setup() {
  // 1. Start Serial Monitor (Great for debugging your pot values)
  Serial.begin(9600);

  // 2. Attach the Servos to Digital Pins
  shoulderServo.attach(9);  // Connect Shoulder Servo to D9
  elbowServo.attach(10);    // Connect Elbow Servo to D10
  wristServo.attach(11);    // Connect Wrist Servo to D11

  // 3. Set Stepper Speed (RPM)
  baseStepper.setSpeed(10);  // 10-15 RPM is a safe sweet spot for 28BYJ-48

  Serial.println("--- Robotic Arm Initialized ---");
}

void loop() {
  // 1. Read the Potentiometers (0 to 1023)
  baseVal = analogRead(basePot);
  shoulderVal = analogRead(shoulderPot);
  elbowVal = analogRead(elbowPot);
  wristVal = analogRead(wristPot);

  // 2. Map values for Servos and Stepper
  int shoulderAngle = map(shoulderVal, 0, 1023, 0, 180);
  int elbowAngle = map(elbowVal, 0, 1023, 0, 180);
  int wristAngle = map(wristVal, 0, 1023, 0, 180);
  int targetStepperPos = map(baseVal, 0, 1023, 0, stepsPerRev);

  // 3. Check for movement and Print to Serial Monitor
  // We use "abs(current - last) > 2" to ignore tiny electrical noise from pots
  if (abs(shoulderAngle - lastShoulder) > 2) {
    Serial.print("Moving Shoulder to: ");
    Serial.println(shoulderAngle);
    lastShoulder = shoulderAngle;
  }

  if (abs(elbowAngle - lastElbow) > 2) {
    Serial.print("Moving Elbow to: ");
    Serial.println(elbowAngle);
    lastElbow = elbowAngle;
  }

  if (abs(wristAngle - lastWrist) > 2) {
    Serial.print("Moving Wrist to: ");
    Serial.println(wristAngle);
    lastWrist = wristAngle;
  }

  if (abs(targetStepperPos - currentStepperPos) > 5) {
    Serial.print("Rotating Base. Target Step: ");
    Serial.println(targetStepperPos);
  }

  // 4. Move Servos
  shoulderServo.write(shoulderAngle);
  elbowServo.write(elbowAngle);
  wristServo.write(wristAngle);

  // 5. Move Stepper (The "Base")
  // Calculate how many steps to move to reach the target
  int stepsToMove = targetStepperPos - currentStepperPos;

  baseStepper.step(stepsToMove);
  currentStepperPos = targetStepperPos;  // Update the record for the next loop

  delay(10);  // Short delay to help stabilize analog readings
}