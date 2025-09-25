#include <Wire.h>
#include <RTClib.h>

// Create an RTC object for the DS3231
RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  
  // Initialize the I2C communication
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); // Stop the program if the RTC isn't found
  }

  // Set the RTC to the current compile time
  // This is a common method for setting the time once
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  // Alternatively, you can manually set a specific date and time:
  // rtc.adjust(DateTime(2020, 9, 18, 10, 10, 18)); // Year, Month, Day, Hour, Minute, Second
}

void loop() {
  // Get the current time from the RTC module
  DateTime now = rtc.now();

  // Print the current date and time
  Serial.print("Current Date / Time: ");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.year());
  Serial.print("  ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  // Delay so the program doesn't print non-stop
  delay(500);
}