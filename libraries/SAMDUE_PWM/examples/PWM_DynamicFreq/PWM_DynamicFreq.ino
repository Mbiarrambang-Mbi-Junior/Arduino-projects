/****************************************************************************************************************************
  PWM_DynamicFreq.ino

  For SAM_DUE boards using hardware-based PWM
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/SAMDUE_PWM
  Licensed under MIT license
*****************************************************************************************************************************/

#define _PWM_LOGLEVEL_       4

// Select false to use PWM
#define USING_TIMER       true

#include "SAMDUE_PWM.h"

// SAM_DUE:
// PWM pins: 6, 7, 8, 9
// Timer pins: 2-5, 10-13.
// pin2:   TC0_CHA0, pin3:  TC2_CHA7, pin4:  TC2_CHB6, pin5:  TC2_CHA6
// pin 10: TC2_CHB7, pin11: TC2_CHA8, pin12: TC2_CHB8, pin13: TC0_CHB0

#if USING_TIMER
  #define pinToUse       5
#else
  #define pinToUse       6
#endif

//creates pwm instance
SAMDUE_PWM* PWM_Instance;

float frequency;

char dashLine[] = "=====================================================================================";

void printPWMInfo(SAMDUE_PWM* PWM_Instance)
{
  Serial.println(dashLine);
  Serial.print("Actual data: pin = ");
  Serial.print(PWM_Instance->getPin());
  Serial.print(", PWM DC = ");
  Serial.print(PWM_Instance->getActualDutyCycle());
  Serial.print(", PWMPeriod = ");
  Serial.print(PWM_Instance->getPWMPeriod());
  Serial.print(", PWM Freq (Hz) = ");
  Serial.println(PWM_Instance->getActualFreq(), 4);
  Serial.println(dashLine);
}

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  delay(500);

#if USING_TIMER
  Serial.print(F("\nStarting PWM_DynamicFreq using Timer on "));
#else
  Serial.print(F("\nStarting PWM_DynamicFreq using PWM on "));
#endif

  Serial.println(BOARD_NAME);
  Serial.println(SAMDUE_PWM_VERSION);

  frequency = 10000.0f;

  PWM_Instance = new SAMDUE_PWM(pinToUse, frequency, 50.0f);

  if ( (!PWM_Instance) || !PWM_Instance->isPWMEnabled())
  {
    Serial.print(F("Stop here forever"));

    while (true)
      delay(10000);
  }

  Serial.println(dashLine);
}

void loop()
{
  delay(5000);

  frequency = 20000.0f;

  Serial.print(F("Change PWM Freq to "));
  Serial.println(frequency);
  PWM_Instance->setPWM(pinToUse, frequency, 50.0f);
  printPWMInfo(PWM_Instance);

  delay(5000);

  frequency = 10000.0f;

  Serial.print(F("Change PWM Freq to "));
  Serial.println(frequency);
  PWM_Instance->setPWM(pinToUse, frequency, 50.0f);
  printPWMInfo(PWM_Instance);
}
