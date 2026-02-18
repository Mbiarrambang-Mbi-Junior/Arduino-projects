/****************************************************************************************************************************
  analogWriteTest.ino

  For SAM_DUE boards using hardware-based PWM
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/SAMDUE_PWM
  Licensed under MIT license
*****************************************************************************************************************************/

#include "Arduino.h"


#define _PWM_LOGLEVEL_       4

// SAM_DUE:
// PWM pins: 6, 7, 8, 9
// Timer pins: 2-5, 10-13. 
// pin2:   TC0_CHA0, pin3:  TC2_CHA7, pin4:  TC2_CHB6, pin5:  TC2_CHA6
// pin 10: TC2_CHB7, pin11: TC2_CHA8, pin12: TC2_CHB8, pin13: TC0_CHB0
#define pinToUse       2    //6

float frequency = 5000.0f;   //1000.0f;

float dutyCycle = 0.0f;

uint8_t channel = 0;

uint8_t resolutionBit;

/////////////////////////////////////////

static uint8_t PWMEnabled = 0;
//static uint8_t TCChanEnabled[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

static inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to) {
  if (from == to)
    return value;
  if (from > to)
    return value >> (from-to);
  else
    return value << (to-from);
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void myAnalogWrite(uint32_t ulPin, uint32_t ulValue) 
{
  static int _writeResolution = 8;
  
  uint32_t attr = g_APinDescription[ulPin].ulPinAttribute;
  
  Serial.print("attr ="); Serial.println(attr);

  if ((attr & PIN_ATTR_PWM) == PIN_ATTR_PWM) {
    ulValue = mapResolution(ulValue, _writeResolution, PWM_RESOLUTION);
    
    Serial.print("ulValue ="); Serial.println(ulValue);

    if (!PWMEnabled) {
      // PWM Startup code
        pmc_enable_periph_clk(PWM_INTERFACE_ID);
        PWMC_ConfigureClocks(PWM_FREQUENCY * PWM_MAX_DUTY_CYCLE, 0, VARIANT_MCK);
      PWMEnabled = 1;
      
      Serial.print("Not PWMEnabled, PWM_FREQUENCY ="); Serial.print(PWM_FREQUENCY);
      Serial.print(", PWM_MAX_DUTY_CYCLE ="); Serial.println(PWM_MAX_DUTY_CYCLE);
    }

    uint32_t chan = g_APinDescription[ulPin].ulPWMChannel;
    if ((g_pinStatus[ulPin] & 0xF) != PIN_STATUS_PWM) {
      // Setup PWM for this pin
      PIO_Configure(g_APinDescription[ulPin].pPort,
          g_APinDescription[ulPin].ulPinType,
          g_APinDescription[ulPin].ulPin,
          g_APinDescription[ulPin].ulPinConfiguration);
      PWMC_ConfigureChannel(PWM_INTERFACE, chan, PWM_CMR_CPRE_CLKA, 0, 0);
      PWMC_SetPeriod(PWM_INTERFACE, chan, PWM_MAX_DUTY_CYCLE);
      PWMC_SetDutyCycle(PWM_INTERFACE, chan, ulValue);
      PWMC_EnableChannel(PWM_INTERFACE, chan);
      g_pinStatus[ulPin] = (g_pinStatus[ulPin] & 0xF0) | PIN_STATUS_PWM;
      
      Serial.print("Setup PWM, chan ="); Serial.print(chan);
      Serial.print(", ulValue ="); Serial.println(ulValue);
    }

    PWMC_SetDutyCycle(PWM_INTERFACE, chan, ulValue);
    return;
  }
}

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  delay(500);

  Serial.println(F("\nStarting analogWriteTest"));
  //Serial.println(BOARD_NAME);

  // Using 8-bit
  resolutionBit = 8;
  
  //analogWriteResolution(resolutionBit);

  //assigns PWM frequency of 1.0 KHz and a duty cycle of 0%
  //analogWrite(uint32_t pin, uint32_t value)
  analogWrite(pinToUse, 0);
  //myAnalogWrite(pinToUse, 0);
}

void loop()
{
  //frequency = 2000.0f;
  dutyCycle = 20.0f;

  //PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);
  analogWrite( 2, 10 *  (1 << resolutionBit) / 100);
  analogWrite( 3, 20 *  (1 << resolutionBit) / 100);
  analogWrite( 4, 30 *  (1 << resolutionBit) / 100);
  analogWrite( 5, 40 *  (1 << resolutionBit) / 100);
  analogWrite(10, 50 *  (1 << resolutionBit) / 100);
  analogWrite(11, 60 *  (1 << resolutionBit) / 100);
  analogWrite(12, 70 *  (1 << resolutionBit) / 100);
  analogWrite(13, 80 *  (1 << resolutionBit) / 100);
  //myAnalogWrite(pinToUse, dutyCycle * (1 << resolutionBit) / 100);

  delay(10000);
  dutyCycle = 90.0f;

  //PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);
  analogWrite( 2, 80 *  (1 << resolutionBit) / 100);
  analogWrite( 3, 70 *  (1 << resolutionBit) / 100);
  analogWrite( 4, 60 *  (1 << resolutionBit) / 100);
  analogWrite( 5, 50 *  (1 << resolutionBit) / 100);
  analogWrite(10, 40 *  (1 << resolutionBit) / 100);
  analogWrite(11, 30 *  (1 << resolutionBit) / 100);
  analogWrite(12, 20 *  (1 << resolutionBit) / 100);
  analogWrite(13, 10 *  (1 << resolutionBit) / 100);
  //myAnalogWrite(pinToUse, dutyCycle * (1 << resolutionBit) / 100);

  //while (1)
    delay(10000);
}
