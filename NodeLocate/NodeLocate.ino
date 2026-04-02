/*
  ## project name: NodeLocate
  ## description:
      This ststem constantly gets the cordinates of the one who wares the bracelet and sends
      the loaction in latitiude and longitiud evry 1 minute through SMS, sending it to a server
      which then processes it and then upload to the cloud for tracking
  ## email: mbiarrambangmbijunior@gmail.com
  ## colaborators:
      Mbiarrambang Mbi Junior
      Mbiarrambang Etchu Ferdinand
      Nyandza Carine
      Lidy Faith
      John Doe

*/

#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h>

const int batteryPin = A0; 

SoftwareSerial ss(4, 3);   // GPS: RX=4, TX=3
SoftwareSerial gsm(8, 7);  // GSM: RX=8, TX=7
TinyGPSPlus gps;

String networkName = "Searching...";
unsigned long lastSMS = 0; // Timer for 1-minute interval

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  gsm.begin(9600);
  lcd.init();

  Serial.println("System Initializing...");
  delay(1000);
  
  gsm.listen(); // Switch focus to GSM for setup
  gsm.println("AT");
  delay(500);
  detectNetwork();
  
  ss.listen(); // Switch back to GPS to start tracking
}

void loop() {
  //Process GPS Data
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      displayFullData();
    }
  }

  //SMS Timer (Every 60 seconds)
  if (millis() - lastSMS > 60000 && gps.location.isValid()) {
    sendLocationSMS();
    lastSMS = millis();
    ss.listen(); // Always return to GPS for tracking after sending
  }

  //Battery status every 10 seconds if no fix
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 10000) {
    if (!gps.location.isValid()) {
      Serial.print("Searching for Satellites... | ");
      Serial.print("Battery: ");
      Serial.print(getBatteryPercentage());
      Serial.println("%");
    }
    lastUpdate = millis();
  }
}

int getBatteryPercentage() {
  int sensorValue = analogRead(batteryPin);
  float vOut = sensorValue * (5.0 / 1023.0);
  float vinVoltage = vOut * 2.0; 
  float actualBatteryVoltage = vinVoltage + 0.7;
  return map(constrain(actualBatteryVoltage * 10, 70, 90), 70, 90, 0, 100);
}

void displayFullData() {
  int satelliteCount = gps.satellites.value();
  if (gps.location.isValid()) {
    Serial.print("Sats: "); Serial.print(satelliteCount);
    Serial.print(" | Lat: "); Serial.print(gps.location.lat(), 6);
    Serial.print(" | Lng: "); Serial.print(gps.location.lng(), 6);
  } else {
    Serial.print("Sats: "); Serial.print(satelliteCount);
    Serial.print(" | Acquiring Fix...");
  }
  Serial.print(" | Battery: ");
  Serial.print(getBatteryPercentage());
  Serial.println("%");
}

void detectNetwork(){
  gsm.listen();
  gsm.println("AT+COPS?");
  delay(1000);
  while(gsm.available()) {
    String response = gsm.readString();
    if (response.indexOf("MTN") != -1) networkName = "MTN";
    else if (response.indexOf("Orange") != -1) networkName = "Orange";
  }
  Serial.print("Network Detected: ");
  Serial.println(networkName);
}

void sendLocationSMS() {
  Serial.println("Attempting to send SMS...");
  gsm.listen(); // Must listen to GSM to send commands
  gsm.println("AT+CMGF=1");
  delay(500);
  gsm.print("AT+CMGS=\""); gsm.print("+237xxxxxxxxx"); gsm.println("\"");
  delay(500);

  gsm.print("Network: "); gsm.println(networkName);
  gsm.print("Lat: "); gsm.print(gps.location.lat(), 6);
  gsm.print(" Lng: "); gsm.print(gps.location.lng(), 6);
  gsm.print(" Bat: "); gsm.print(getBatteryPercentage()); gsm.println("%");

  delay(500);
  gsm.write(26); // CTRL+Z
  delay(5000); // Give it time to send
  Serial.println("SMS Sent!");
}
