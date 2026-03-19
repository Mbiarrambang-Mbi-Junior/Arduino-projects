//#include <TinyGPS.h>

#include <TinyGPSPlus.h>

// --- Pin Definitions ---
// Use 'const' to save memory and prevent accidental changes
const int GPS_RX_PIN = 12; 
const int GPS_TX_PIN = 14;
const long GPS_BAUD = 9600;

TinyGPSPlus gps;

// Define Serial2 for the GPS
#define gpsSerial Serial2

void setup() {
  Serial.begin(115200);

  // Using the variables here instead of hard-coded numbers
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  Serial.println("Waiting for GPS fix and satellites...");
  Serial.print("GPS Pins: RX=");
  Serial.print(GPS_RX_PIN);
  Serial.print(", TX=");
  Serial.println(GPS_TX_PIN);
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayLocationInfo();
    }
  }

  // Basic check to see if we are getting any data at all
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring or pin variables."));
    while (true);
  }

  delay(1000);
}

void displayLocationInfo() {
  // ... (rest of your displayLocationInfo function remains the same)
  Serial.println(F("-------------------------------------"));
  Serial.print("Latitude:  ");
  Serial.println(gps.location.lat(), 6);
  Serial.print("Longitude: ");
  Serial.println(gps.location.lng(), 6);
  Serial.print("Satellites: ");
  Serial.println(gps.satellites.value());
  Serial.println(F("-------------------------------------"));
}