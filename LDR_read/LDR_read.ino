// Pin Definitions
const int LDR_PIN = A0;     // LDR connected to Analog A0
const int OPTO_PIN = 13;    // Arduino Pin 13 to Optocoupler input

// Calibration
int threshold = 500;        // Adjust this value based on simulation light level

void setup() {
  pinMode(OPTO_PIN, OUTPUT); // Set pin 13 as output
  Serial.begin(9600);        // To monitor values in the Virtual Terminal
}

void loop() {
  int lightLevel = analogRead(LDR_PIN); // Read light value (0 to 1023)
  
  // Print value to serial monitor for debugging
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  // LOGIC: If light falls below threshold (gets dark), turn on relay
  // If you want "Resistance Decreases = ON", change to: if (lightLevel > threshold)
  if (lightLevel < threshold) {
    digitalWrite(OPTO_PIN, HIGH); // Send signal to Optocoupler -> Relay ON
  } else {
    digitalWrite(OPTO_PIN, LOW);  // Relay OFF
  }

  delay(200); // Small delay for stability
}