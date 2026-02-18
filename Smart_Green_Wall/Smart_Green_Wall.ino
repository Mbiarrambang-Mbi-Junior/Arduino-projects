/*
 * Project: IoT Smart Green Wall Controller
 * Organization: Paveway Group Cameroon
 * Description: Automated ventilation and moisture monitoring using 
 * MQTT for real-time telemetry and SD card for local logging.
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>

// --- Pin Mapping ---
#define SOIL_PIN 34 // Analog pin for Soil Moisture
#define GAS_PIN  35 // Analog pin for MQ-135 Gas Sensor
#define FAN_PIN  12 // Digital pin for Fan Control(via Transistor/Relay)
#define SD_CS       5    // Chip Select for SD Card
#define LED_ALERT   27   

// --- Thresholds ---
const int DRY_SOIL = 1500; // Threshold for low moisture (ADC value)
const int GAS_LIMIT = 800; // Threshold for air quality alert

// --- Objects ---
WiFiClient espClient;
PubSubClient client(espClient);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  
  // Initialize Pins
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_ALERT, OUTPUT);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Paveway IoT...");

  // Initialize SD Card
  if(!SD.begin(SD_CS)) {
    Serial.println("SD Card Failed!");
  }

  // WiFi Setup
  WiFi.begin("Paveway_AP", "Admin123");
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  
  client.setServer("192.168.1.100", 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // 1. Read Sensors
  int moisture = analogRead(SOIL_PIN);
  int gasLevel = analogRead(GAS_PIN);

  // 2. Logic: Air Quality Control
  if (gasLevel > GAS_LIMIT) {
    digitalWrite(FAN_PIN, HIGH);  // Activate Fan
    digitalWrite(LED_ALERT, HIGH);
    lcd.setCursor(0,0);
    lcd.print("AIR: ALERT!   ");
  } else {
    digitalWrite(FAN_PIN, LOW);   // Deactivate Fan
    digitalWrite(LED_ALERT, LOW);
    lcd.setCursor(0,0);
    lcd.print("AIR: CLEAN    ");
  }

  // 3. Logic: Moisture Monitoring
  lcd.setCursor(0,1);
  lcd.print("Soil: ");
  lcd.print(moisture);

  // 4. Data Persistence (SD Card Logging)
  File dataFile = SD.open("/logs.csv", FILE_APPEND);
  if (dataFile) {
    dataFile.printf("%d,%d\n", moisture, gasLevel);
    dataFile.close();
  }

  // 5. MQTT Telemetry
  String payload = "{\"moist\":" + String(moisture) + ",\"gas\":" + String(gasLevel) + "}";
  client.publish("paveway/greenwall/telemetry", payload.c_str());

  delay(2000); // 2-second polling interval
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("GreenWallNode")) {
      client.subscribe("paveway/greenwall/cmd");
    } else {
      delay(5000);
    }
  }
}

