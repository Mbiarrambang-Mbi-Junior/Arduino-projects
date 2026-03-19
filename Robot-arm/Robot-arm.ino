#include <Servo.h>

int potPin = 0;  
int pos = 0;
  
int button = 2;
int servoPin = 9;

int angle = 0; 

const int servo1 = 3; 

const int servo2 = 10; 

const int joyH = 3; 

const int joyV = 4;

int servoVal;
 
Servo servo;

Servo myservo1; 

Servo myservo2;

Servo myservo3;

void setup() {

 myservo3.attach(6);  
 pinMode(pos, OUTPUT);
 pinMode(button, INPUT); 
 digitalWrite (button, LOW);

 servo.attach(servoPin);

 myservo1.attach(servo1);
 myservo2.attach(servo2); 


 Serial.begin(9600);

}

void loop(){
  
 int reading = analogRead(potPin); 

 int angle = reading / 6; 

 servo.write(angle);

 servoVal = analogRead(joyH);

 servoVal = map(servoVal, 0, 1023, 0, 180); 

 myservo2.write(servoVal); 

 servoVal = analogRead(joyV);

 servoVal = map(servoVal, 0, 1023, 70, 180); 

 myservo1.write(servoVal);

    if (digitalRead(button) == LOW)

  for(pos = 90; pos >= 90; pos -= 90) 
  {                                  
    myservo3.write(pos);              
                         
  } 
  if (digitalRead(button) == HIGH) 
  
  for(pos = 0; pos < 90; pos += 90)     
  {                                
     myservo3.write(pos);              
  delay(50);                            
  }

 delay(15); 

}


