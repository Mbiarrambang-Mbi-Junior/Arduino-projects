int greenpin = 11;
int redpin = 9;
int bluepin = 3;


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(greenpin, OUTPUT);
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
}

 // the loop function runs over and over again forever
void loop() {
  digitalWrite(greenpin, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);  
                      // wait for a second
  digitalWrite(greenpin, LOW);
  delay(1000);

  digitalWrite(redpin, HIGH);   // turn the LED off by making the voltage LOW
  delay(1000);  

  digitalWrite(redpin, LOW);   // turn the LED off by making the voltage LOW
  delay(1000); 

  digitalWrite(bluepin, HIGH);                   // wait for a second
  delay(8000);
  
  digitalWrite(bluepin, LOW);   // turn the LED off by making the voltage LOW
  delay(1000); 
  
}
