// Automatic security light detector
// is used an security lights in homes and even street lights when places are dark

const int LDR_PIN = A0;
const int OPTO_PIN = 11;
int threshold = 300; // 0 will trigger the 'ON' state, 300 threshold = 7.1 in voltage in the ptoteus simulator

void setup() {
  pinMode(OPTO_PIN, OUTPUT); // set the optocoupler as an output device
}

void loop() {
  int val = analogRead(LDR_PIN);
  
  // Resistance decreases (value goes UP) when it gets brighter
  if (val < threshold) {
    digitalWrite(OPTO_PIN, HIGH); // Turn on lamp through a relay donected to the secondary side of the octocoupler
  } else {
    digitalWrite(OPTO_PIN, LOW);  // Turn off lamp
  }
}