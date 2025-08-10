#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16,2);

#include<Servo.h>
int x_pin = A0;  //X-axis pin
int y_pin = A1;
int sw_pin = 2;

int pos1 = 0;
int pos2 = 0;

Servo s1;
Servo s2;

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);

  s1.attach(3);          //Servo motor attached to pin 3
  s1.write(pos1);
  s2.attach(5);
  s2.write(pos2);
  lcd.init();
  lcd.backlight();


}

void loop()
{
  lcd.setCursor(0,0);
  lcd.print("hi i am junior");
 int x_data = analogRead(A0);
 int y_data = analogRead(A1);
 int sw_data = digitalRead(2);

 Serial.print("x_data:");
 Serial.print(x_data);
 Serial.print("\t");
 Serial.print("y_data:");
 Serial.print(y_data);
 Serial.print("\t");
 Serial.print("sw_data:");
 Serial.println(sw_data);
 delay(80);

if(x_data >= 550)
{
  if(pos1 <= 180)
  {
    lcd.print("x-axis");
  pos1 = pos1 + 20;
  s1.write(pos1);
  }
}
else if(x_data <= 500)
{
  if(pos1 >= 0)
  {
   pos1 = pos1 - 20;
   s1.write(pos1);
  }
}

if(y_data >= 550)
{
  lcd.print("y-axis");
  if(pos2 <= 180)
  {
  pos2 = pos2 + 20;
  s2.write(pos2);
  }
}
else if(y_data <= 500)
{
  if(pos2 >= 0)
  {
   pos2 = pos2 - 20;
   s2.write(pos2);
  }
}
  
}
