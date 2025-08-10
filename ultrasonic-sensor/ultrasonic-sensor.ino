// Define the pins for the ultrasonic sensor
const int trigPin = 9;  // Trigger pin
const int echoPin = 10; // Echo pin

// Define variables for duration and distance
long duration; // Variable to store the duration of the sound wave travel
int distanceCm; // Variable to store the distance in centimeters

void setup() {
  // Set the trigger pin as an OUTPUT
  pinMode(trigPin, OUTPUT);
  // Set the echo pin as an INPUT
  pinMode(echoPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor HC-SR04 Test");
  Serial.println("------------------------------------");
}

void loop() {
  // 1. Clear the trigPin by setting it LOW for a moment
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // 2. Set the trigPin HIGH for 10 microseconds to send a pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 3. Measure the duration of the pulse on the echoPin
  // pulseIn() returns the duration (length of the pulse) in microseconds
  duration = pulseIn(echoPin, HIGH);

  // 4. Calculate the distance
  // Speed of sound in air at 20°C is approximately 343 meters/second, or 0.0343 cm/microsecond.
  // The sound travels to the object and back, so we divide the duration by 2.
  distanceCm = duration * 0.034 / 2;

  // 5. Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  // Add a small delay between measurements to avoid erratic readings
  delay(100);
  if (distanceCm >=100 && distanceCm <=700)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}