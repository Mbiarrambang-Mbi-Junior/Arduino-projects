// --- Pin Definitions ---
const int soundSensorPin = 2; // Must be an Interrupt Pin (D2 or D3 on Uno)
const int gasSensorPin = A0;   // Analog Pin for Gas Level
const int systemLed = 13;      // Status LED

// --- Timing and Logic Variables ---
volatile bool soundDetected = false;
unsigned long lastTriggerTime = 0;
const unsigned long lockOutInterval = 1500; // 1.5s Lockout to ignore echoes
const int gasThreshold = 400;               // Calibrated threshold

void setup() {
  Serial.begin(9600);
  pinMode(soundSensorPin, INPUT_PULLUP);
  pinMode(systemLed, OUTPUT);

  // 1. Implementation: Attach Hardware Interrupt
  // Triggers ISR immediately when sound sensor goes LOW
  attachInterrupt(digitalPinToInterrupt(soundSensorPin), soundISR, FALLING);

  Serial.println("System Initializing...");
  Serial.println("Waiting 60s for Gas Sensor warm-up...");
  // delay(60000); // Uncomment for real industrial deployment
}

void loop() {
  // 2. Process Sound Event (Acoustic Trigger)
  if (soundDetected) {
    handleAcousticReset();
  }

  // 3. Monitor Gas Sensor (Environmental Safety)
  int gasValue = analogRead(gasSensorPin);
  if (gasValue > gasThreshold) {
    Serial.print("ALERT: Gas Leak Detected! Value: ");
    Serial.println(gasValue);
    digitalWrite(systemLed, HIGH); // Visual Alarm
  } else {
    digitalWrite(systemLed, LOW);
  }
}

// --- Interrupt Service Routine (ISR) ---
void soundISR() {
  soundDetected = true; // Keep ISR extremely short
}

void handleAcousticReset() {
  unsigned long currentTime = millis();

  // Troubleshooting: Software Lockout Timer (Debouncing)
  if (currentTime - lastTriggerTime > lockOutInterval) {
    Serial.println("Acoustic Pattern Verified: Triggering System Reset...");
    
    // Simulate System Reset Logic
    digitalWrite(systemLed, HIGH);
    delay(200);
    digitalWrite(systemLed, LOW);

    lastTriggerTime = currentTime; // Update last valid trigger
  }
  
  soundDetected = false; // Clear the flag
}
