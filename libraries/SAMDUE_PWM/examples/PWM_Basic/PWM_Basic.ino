/****************************************************************************************************************************
  PWM_Basic.ino

  For SAM_DUE boards using hardware-based PWM
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/SAMDUE_PWM
  Licensed under MIT license
*****************************************************************************************************************************/

#define _PWM_LOGLEVEL_       4

// Select false to use PWM
#define USING_TIMER       false   //true

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

float frequency = 1000.0f;

float dutyCycle = 0.0f;

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  delay(500);

#if USING_TIMER
  Serial.print(F("\nStarting PWM_Basic using Timer on "));
#else
  Serial.print(F("\nStarting PWM_Basic using PWM on "));
#endif

  Serial.println(BOARD_NAME);
  Serial.println(SAMDUE_PWM_VERSION);

  //assigns PWM frequency of 1.0 KHz and a duty cycle of 0%
  PWM_Instance = new SAMDUE_PWM(pinToUse, frequency, dutyCycle);

  if ( (!PWM_Instance) || !PWM_Instance->isPWMEnabled())
  {
    Serial.print(F("Stop here forever"));

    while (true)
      delay(10000);
  }
}

void loop()
{
  // You can change frequency here, anytime
  frequency = 2000.0f;
  dutyCycle = 20.0f;

  PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);

  delay(10000);

  // You can change frequency here, anytime
  frequency = 5000.0f;
  dutyCycle = 90.0f;

  PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);

  //while (1)
  delay(10000);
}
