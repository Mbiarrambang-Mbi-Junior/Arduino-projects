#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>

// Initialize sensors
Adafruit_BMP280 bmp; // Default Address 0x76
Adafruit_BME280 bme; // Default Address 0x77

void setup() {
  Serial.begin(115200); // High speed for SLAM-ready telemetry
  Serial.println("Initializing Environmental Hub...");

  // Start BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("Check BMP280 wiring/address!");
  }

  // Start BME280
  if (!bme.begin(0x77)) {
    Serial.println("Check BME280 wiring/address!");
  }

  Serial.println("Bus Contention Resolved. Streaming Data...");
}

void loop() {
  // Read from BMP280 (Node A)
  float tempA = bmp.readTemperature();
  
  // Read from BME280 (Node B)
  float tempB = bme.readTemperature();
  float humidityB = bme.readHumidity();

  // SERIALIZATION: Formating for "Cloud" or "Master Node" processing
  // Format: [NodeID],[Temp],[Humidity]
  Serial.print("NODE_A,"); Serial.print(tempA); Serial.println(",NULL");
  Serial.print("NODE_B,"); Serial.print(tempB); Serial.print(","); Serial.println(humidityB);

  delay(1000); 
}