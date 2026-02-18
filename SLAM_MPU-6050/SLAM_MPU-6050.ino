#include <Wire.h>

const int MPU = 0x68; // I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY;
float roll, pitch;
float elapsedTime, currentTime, previousTime;

void setup() {
  Serial.begin(115200);
  Serial.println("System Booting..."); // If you don't see this, it's a Baud Rate issue.

  Wire.begin();
  Serial.println("I2C Bus Started..."); // If you see this, Wire.begin worked.

  Wire.beginTransmission(0x68);
  if (Wire.endTransmission() != 0) {
    Serial.println("IMU NOT FOUND! Check SDA/SCL wiring.");
    while(1); // Stop here if sensor is missing
  }
  Serial.println("IMU Connected! Starting data stream...");
}

void loop() {
  // 1. Read Accelerometer Data
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

  // 2. Calculate Accelerometer Angles
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI);
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI);

  // 3. Read Gyroscope Data
  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000; 

  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 4, true);
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;

  // 4. COMPLEMENTARY FILTER (The SLAM Core)
  // Combining high-pass (gyro) and low-pass (accel) filters
  roll = 0.96 * (roll + GyroX * elapsedTime) + 0.04 * accAngleX;
  pitch = 0.96 * (pitch + GyroY * elapsedTime) + 0.04 * accAngleY;

  // 5. Output for Cloud Processing
  Serial.print("Roll:"); Serial.print(roll);
  Serial.print(",");
  Serial.print("Pitch:"); Serial.println(pitch);
}