#include <Servo.h>
#include <SoftwareSerial.h>


//DefineSoftwareSerialpinsforHC-05communication
//NOTE:ThesepinsarenotD0/D1,eliminatingtheconflict.
const int bluetoothRx = 11;  //ConnectstoHC-05TX
const int bluetoothTx = 12;  //ConnectstoHC-05RX
SoftwareSerial bluetooth(bluetoothRx, bluetoothTx);

Servo myservo;

//Yourexistingpindefinitions
int position = 0;
int livingroom = 2;
int doorLock = 9;  //Servocontrolpin
int bedroom = 10;

char incomingValue = 0;

void setup() {
  Serial.begin(9600);     //ONLYfordebuggingoutputtothePCSerialMonitor
  bluetooth.begin(9600);  //**HC-05communicationnowstartshere**

  myservo.attach(doorLock);
  pinMode(livingroom, OUTPUT);
  pinMode(bedroom, OUTPUT);

  Serial.println("Smart Home System Ready");
}

void loop() {
  //ChecktheBluetoothserialportinsteadofthehardwareSerialport
  if (bluetooth.available()) {
    incomingValue = bluetooth.read();

    //------------------
    //Servo/DoorLockControl
    //...yourservologicremainsthesame...
    if (incomingValue == '6') {
      for (position = 0; position <= 90; position += 1) {
        myservo.write(position);
        delay(15);
      }
      Serial.println("Door UNLOCKED");
    }

    if (incomingValue == '7') {
      for (position = 90; position >= 0; position -= 1) {
        myservo.write(position);
        delay(15);
      }
      Serial.println("Door LOCKED");
    }
    //------------------

    //LightControls(Sameasbefore)
    if (incomingValue == '4') {
      digitalWrite(livingroom, HIGH);
      Serial.println("LivingRoom Light ON");
    }
    if (incomingValue == '1') {
      digitalWrite(livingroom, LOW);
      Serial.println("LivingRoom Light OFF");
    }
    if (incomingValue == '5') {
      digitalWrite(bedroom, HIGH);
      Serial.println("BedroomLight ON");
    }
    if (incomingValue == '2') {
      digitalWrite(bedroom, LOW);
      Serial.println("BedroomLight OFF");
    }
    if (incomingValue == '9') {
      digitalWrite(livingroom, HIGH);
      digitalWrite(bedroom, HIGH);
      Serial.println("ALLLIGHT'S ON");
    }
    if (incomingValue == '0') {
      digitalWrite(livingroom, LOW);
      digitalWrite(bedroom, LOW);
      Serial.println("ALLLIGHT'S OFF");
    }
  }
}