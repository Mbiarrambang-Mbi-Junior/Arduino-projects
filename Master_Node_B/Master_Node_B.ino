#include <SoftwareSerial.h>

// RX on Pin 2, TX on Pin 3 (We only need RX for this node)
SoftwareSerial LinkFromAuth(2, 3); 

void setup() {
  Serial.begin(9600);         // To PC Serial Monitor
  LinkFromAuth.begin(9600);   // From the other Arduino
  Serial.println("MASTER NODE: Waiting for Auth Packets...");
}

void loop() {
  if (LinkFromAuth.available()) {
    // Read the serialized CSV string
    String incomingMessage = LinkFromAuth.readStringUntil('\n'); 
    
    // DESERIALIZATION (Breaking the packet)
    int commaIndex = incomingMessage.indexOf(',');
    if (commaIndex != -1) {
      String uid = incomingMessage.substring(0, commaIndex);
      String value = incomingMessage.substring(commaIndex + 1);

      // Display the structured data
      Serial.println("--- NEW DATA RECEIVED ---");
      Serial.print("Operator UID: "); Serial.println(uid);
      Serial.print("Moisture Level: "); Serial.print(value);
      Serial.println("\n-------------------------");
    }
  }
}