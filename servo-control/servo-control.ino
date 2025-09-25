#include <Servo.h>

Servo servo1;
Servo servo2;  
int joyX = 0;
int joyY = 1;
int joyval;

void setup() {
  servo1.attach(9);  // attaches the servo on pin 9 to the servo object
  servo2.attach(6);
}

void loop() {
  joyval = analogRead(joyX);
  joyval = map (joyval, 0, 1023, 0, 180);//servo value betweem 0-180
  servo1.write(joyval);//set the servo position according to the joystick value
  
  joyval = analogRead(joyY);
  joyval = map (joyval, 0, 1023, 0, 180);
  servo2.write(joyval);
  
}