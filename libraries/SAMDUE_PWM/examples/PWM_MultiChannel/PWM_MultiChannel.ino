/****************************************************************************************************************************
  PWM_MultiChannel.ino

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
uint32_t PWM_Pins[]   = { 2, 3, 4, 5 };
#else
uint32_t PWM_Pins[]   = { 6, 7, 8, 9 };
#endif

#define NUM_OF_PINS       ( sizeof(PWM_Pins) / sizeof(uint32_t) )

float dutyCycle[] = { 10.0f, 30.0f, 50.0f, 90.0f };

// Must be same frequency for same channel
float frequency = 2000.0f;

//creates pwm instances
SAMDUE_PWM* PWM_Instance[NUM_OF_PINS];

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
  Serial.print(F("\nStarting PWM_MultiChannel using Timer on "));
#else
  Serial.print(F("\nStarting PWM_MultiChannel using PWM on "));
#endif

  Serial.println(BOARD_NAME);
  Serial.println(SAMDUE_PWM_VERSION);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    PWM_Instance[index] = new SAMDUE_PWM(PWM_Pins[index], frequency, dutyCycle[index]);

    if (PWM_Instance[index])
    {
      PWM_Instance[index]->setPWM();
    }
  }

  Serial.println(dashLine);
  Serial.println("Index\tPin\tPWM_freq\tDutyCycle\tActual Freq");
  Serial.println(dashLine);

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    if (PWM_Instance[index])
    {
      Serial.print(index);
      Serial.print("\t");
      Serial.print(PWM_Pins[index]);
      Serial.print("\t");
      Serial.print(frequency);
      Serial.print("\t\t");
      Serial.print(dutyCycle[index]);
      Serial.print("\t\t");
      Serial.println(PWM_Instance[index]->getActualFreq(), 4);
    }
    else
    {
      Serial.println();
    }
  }

  for (uint8_t index = 0; index < NUM_OF_PINS; index++)
  {
    printPWMInfo(PWM_Instance[index]);
  }
}

void loop()
{
  //Long delay has no effect on the operation of hardware-based PWM channels
  delay(1000000);
}
