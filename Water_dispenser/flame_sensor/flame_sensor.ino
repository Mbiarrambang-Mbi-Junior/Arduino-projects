#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
int LED_red = 2;
int LED_green = 3;
int alarm = 4;
int Flame_sensor = 9;
int relay = 12;






void setup() {
  
  lcd.init();
  lcd.backlight();
  pinMode(Flame_sensor,INPUT);
  pinMode(LED_red,OUTPUT);
  pinMode(LED_green,OUTPUT);
  pinMode(alarm,OUTPUT);
  pinMode(relay,OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("EPS ENGINEER");
  lcd.setCursor(0,1);
  lcd.print("FIRE PROJECT");
  delay(1000);
  lcd.clear();
  // put your setup code here, to run once:

}

void loop() {
  int flame_reading = digitalRead(Flame_sensor);
  if (flame_reading == HIGH){
  digitalWrite(LED_red,HIGH);
  digitalWrite(LED_green,LOW);
  tone(alarm,3000,300);
  digitalWrite(relay,HIGH);
  lcd.setCursor(0,0);
  lcd.print("ALERT FIRE IN          ");
  lcd.setCursor(0,1);
  lcd.print("IN ROOM 1            ");
  }


  
  else if(flame_reading == LOW){
  
  digitalWrite(LED_red,LOW);
  digitalWrite(LED_green,HIGH);
  digitalWrite(alarm,LOW);
  digitalWrite(relay,LOW);
  lcd.setCursor(0,0);
  lcd.print("NORMAL OPERATION"     );
  lcd.setCursor(0,1);
  lcd.print("IN ROOM 1           ");
  }
  
  
  
  // put your main code here, to run repeatedly:

}
