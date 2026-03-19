/*
OmniSol Solar Tracker
OMNi Technologies
Designed and fabricated by Om Anavekar
Made in the USA
9/12/18
*/

#include <Servo.h> // Servo library

//Servos
Servo trackerLR;  // Create servo object for left/right movement servo 
Servo trackerTB;  // Create servo object for top/bottom movement servo 

//Photoresistor Pins
int rightLDRpin = A0;  //Analog pins for photoresistors
int leftLDRpin = A1;
int bottomLDRpin = A2;

//Photoresistors
int rightLDR = 0;  //Variables for the sensor values
int leftLDR = 0;
int bottomLDR = 0;
int topLDR = 0;    //Not a real sensor. This will be the average of right and left photoresistors.

//Differences between right/left and top/bottom photoresistors
int horizontalError = 0;
int verticalError = 0;

int trackerLRPos = 90;    //Create a variable to store the servo position
int trackerTBPos = 90;

void setup() 
{ 
 //Servo pins
 trackerLR.attach(2);  
 trackerTB.attach(3);

}
 
void loop() 
{ 
  //Values of the right and left sensors
  rightLDR = analogRead(rightLDRpin);    
  leftLDR = analogRead(leftLDRpin);
  
  horizontalError = rightLDR - leftLDR;  //Difference between the two sensors.
  
  if(horizontalError>20)  //If the error is greater than 20 then move the tracker to the right
  {
      trackerLRPos--;
      trackerLRPos = constrain (trackerLRPos, 0,179);
      trackerLR.write(trackerLRPos);  
  }
  
  else if(horizontalError<-20)  //If the error is less than -20 then move the tracker to the left
  {
      trackerLRPos++;
      trackerLRPos = constrain (trackerLRPos, 0,179 );
      trackerLR.write(trackerLRPos);  
  }

  //Values of the bottom and top sensors (top is not real)
  topLDR = (rightLDR + leftLDR)/2; //Average value of right and left sensors
  bottomLDR = analogRead(bottomLDRpin);

  verticalError = topLDR - bottomLDR; //Difference between top and bottom readings
  
  if(verticalError>50)  //If the error is greater than 50 then move the tracker up
  {
      trackerTBPos++;
      trackerTBPos = constrain (trackerTBPos, 10,120);
      trackerTB.write(trackerTBPos);  
  }
  
  else if(verticalError<-50)  //If the error is less than -50 then move the tracker down
  {
      trackerTBPos--;
      trackerTBPos = constrain (trackerTBPos, 10,120);
      trackerTB.write(trackerTBPos); 
  }
  delay(25);
}
