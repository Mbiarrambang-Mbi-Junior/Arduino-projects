#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

int sensor = A0;
int pump = 11;

void setup() {
  pinMode(A0,INPUT);
  pinMode(11,OUTPUT);

 lcd.init();
 lcd.backlight();
}

void loop() {

  lcd.setCursor(1, 0);
  sensor = analogRead(A0);

   if(sensor > 0)
   {
    digitalWrite(pump, HIGH);
    lcd.print("Puring...");
   }
  else
  {
    digitalWrite(pump, LOW);
    lcd.print("Stoped");
  }

}
