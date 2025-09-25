// define the analog pin where the sound sensor's analog input is collected
const int soundSensorpin = A0;

//define the pin where the output or tobe controlled device is put
const int devicePin = 2;

/*
 defin a threshold for sound detection.
 this value vary depending on your sensor and environment.
 you might need to ajust based on your sensor readings in the serial monitor
 */
 const int soundThreshold = 500;//adjustable according to your sensor noise level

 //variable to keep track of the device's current state in boolean (true = ON, false = OFF)
 bool deviceState = false;

 //variable to store the last time the device toggle or change state
 unsigned long lastToggleTime = 0;

 //debounce delay prevents the rapid toggling from continuous sound
 //this means the device will wait for some time befor toggling again
 const unsigned long debounceDelay = 100;//1 secon in milliseconds & you can change to a desired value

void setup() {
  // initialise serial communication for debuging and monitoring sensor values
  Serial.begin(9600);
  Serial.println("Arduino Sound Sensor Device Controle");
  
  //set the device pin
  pinMode(devicePin, OUTPUT);

  //initially turn device OFF
  digitalWrite(devicePin, LOW);
  Serial.println("Device is initialy oFF.");

}

void loop() {
  // read the value from the sound sensor at A0
  int soundValue = analogRead(soundSensorpin);

  // print the raw sensor value you see on the serial monitor for calibration/debugging
  Serial.print("Sound Sensor Value:");
  Serial.println(soundValue);

  //check if the sound value exceeds the threshold value(noise value) and if enough time has passed since the last 
  //toggle
    if (soundValue > soundThreshold && (millis() - lastToggleTime > debounceDelay))
     {
      //a sound was detected above threshold value, and  it's time to toggle the device
      //to toggle device state
      deviceState = !deviceState;

      //update the state of the device
      if(deviceState) {
        digitalWrite(devicePin, HIGH);//turn on
        Serial.println("Sound detected!! Device turned ON.");
      } else {
        digitalWrite(devicePin, LOW);//turn off
        Serial.println("Sound detected!! Device turned OFF.");
      }

      //record the time of this toggle
      lastToggleTime = millis();
     }

     /* 
     a delay to prevent the loop from running too fast,
     which can make the serial output onreadable and consume unnecessary power.
     */
    delay(50);

}
