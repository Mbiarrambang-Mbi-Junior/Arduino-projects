/****************************************************************************************************************************
  SAMDUE_PWM.h

  For SAM_DUE boards using hardware-based PWM
  Based on SAM_DUE analogWrite
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/SAMDUE_PWM
  Licensed under MIT license

  Version: 1.0.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      04/11/2022 Initial coding for SAM_DUE boards
  1.0.1   K Hoang      22/01/2023 Add `PWM_StepperControl` example
*****************************************************************************************************************************/

#pragma once

#ifndef SAMDUE_PWM_H
#define SAMDUE_PWM_H

#if ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  #if !defined(BOARD_NAME)
    #define BOARD_NAME      "SAM_DUE"
  #endif
#else
  #error This is designed only for Arduino megaAVR board! Please check your Tools->Board setting.
#endif

#if (_PWM_LOGLEVEL_ > 3)
  #warning Using SAM_DUE Hardware PWM
#endif

////////////////////////////////////////

#include "Arduino.h"

#ifndef SAMDUE_PWM_VERSION
  #define SAMDUE_PWM_VERSION             "SAMDUE_PWM v1.0.1"

  #define SAMDUE_PWM_VERSION_MAJOR       1
  #define SAMDUE_PWM_VERSION_MINOR       0
  #define SAMDUE_PWM_VERSION_PATCH       1

  #define SAMDUE_PWM_VERSION_INT         1000001
#endif

#include "PWM_Generic_Debug.h"

////////////////////////////////////////

/*************************************
  // In Arduino.h
  #define PIN_STATUS_PWM                   (0x05)

  // Definitions for PWM channels
  typedef enum _EPWMChannel
  {
    NOT_ON_PWM = -1,
    PWM_CH0 = 0,
    PWM_CH1,
    PWM_CH2,
    PWM_CH3,
    PWM_CH4,
    PWM_CH5,
    PWM_CH6,
    PWM_CH7
  } EPWMChannel ;

  // Definitions for TC channels
  typedef enum _ETCChannel
  {
    NOT_ON_TIMER = -1,
    TC0_CHA0 = 0,
    TC0_CHB0,
    TC0_CHA1,
    TC0_CHB1,
    TC0_CHA2,
    TC0_CHB2,
    TC1_CHA3,
    TC1_CHB3,
    TC1_CHA4,
    TC1_CHB4,
    TC1_CHA5,
    TC1_CHB5,
    TC2_CHA6,
    TC2_CHB6,
    TC2_CHA7,
    TC2_CHB7,
    TC2_CHA8,
    TC2_CHB8
  } ETCChannel ;
*************************************/

////////////////////////////////////////

#define INVALID_SAMDUE_PIN          255

////////////////////////////////////////

#define MAX_COUNT_8BIT            256UL
#define MAX_COUNT_16BIT           65536UL

#define MAX_8BIT                  255UL
#define MAX_16BIT                 65535UL

////////////////////////////////////////

static uint8_t _TCChanEnabled[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

static const uint32_t channelToChNo[] = { 0, 0, 1, 1, 2, 2, 0, 0, 1, 1, 2, 2, 0, 0, 1, 1, 2, 2 };
static const uint32_t channelToAB[]   = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };

static Tc *channelToTC[] =
{
  TC0, TC0, TC0, TC0, TC0, TC0,
  TC1, TC1, TC1, TC1, TC1, TC1,
  TC2, TC2, TC2, TC2, TC2, TC2
};

static const uint32_t channelToId[] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8 };

////////////////////////////////////////
////////////////////////////////////////

class SAMDUE_PWM
{
  public:

    // dutycycle = 0.0f - 100.0f => 0-65535
    SAMDUE_PWM(const uint32_t& pin, const float& frequency, const float& dutycycle)
    {
      _dutycycle  = round(map(dutycycle, 0, 100.0f, 0, MAX_COUNT_16BIT));

      // input DC is 16-bit
      setupPWM(pin, (uint32_t) frequency, _dutycycle);
    }

    ///////////////////////////////////////////

    ~SAMDUE_PWM();

    ///////////////////////////////////////////
    ///////////////////////////////////////////

  private:

    // input dutycycle is 16-bit
    bool setupPWM(uint32_t pin, uint32_t frequency, uint16_t dutycycle)
    {
      _pinAttr = g_APinDescription[pin].ulPinAttribute;
      _pin     = pin;

      if (_pinAttr & PIN_ATTR_PWM)
      {
        _dutycycle = dutycycle;

        PWM_LOGDEBUG7(F("setupPWM: _pin ="), _pin, F(", _pinAttr ="), _pinAttr, F(", frequency ="), frequency,
                      F(", _dutycycle ="), _dutycycle);

        if (!_PWMEnabled)
        {
          // PWM Startup code
          pmc_enable_periph_clk(PWM_INTERFACE_ID);
          PWMC_ConfigureClocks(frequency * PWM_MAX_DUTY_CYCLE, 0, VARIANT_MCK);

          _frequency = frequency;

          _PWMEnabled = true;

          PWM_LOGDEBUG1(F("setupPWM: Not PWMEnabled, PWMC_ConfigureClocks, frequency ="), frequency);
        }

        if ( frequency != _frequency)
        {
          PWM_LOGDEBUG3(F("setPWM_Int: change frequency to"), frequency, F("from"), _frequency);

          PWMC_ConfigureClocks(frequency * PWM_MAX_DUTY_CYCLE, 0, VARIANT_MCK);

          _frequency = frequency;
        }

        _channel = g_APinDescription[_pin].ulPWMChannel;

        if ((g_pinStatus[_pin] & 0xF) != PIN_STATUS_PWM)
        {
          // Enable PWM for this pin
          PIO_Configure(g_APinDescription[_pin].pPort, g_APinDescription[_pin].ulPinType,
                        g_APinDescription[_pin].ulPin, g_APinDescription[_pin].ulPinConfiguration);

          PWMC_ConfigureChannel(PWM_INTERFACE, _channel, PWM_CMR_CPRE_CLKA, 0, 0);
          PWMC_SetPeriod(PWM_INTERFACE, _channel, PWM_MAX_DUTY_CYCLE);

          PWMC_EnableChannel(PWM_INTERFACE, _channel);
          g_pinStatus[_pin] = (g_pinStatus[_pin] & 0xF0) | PIN_STATUS_PWM;

          PWM_LOGDEBUG3(F("setupPWM: new _pin ="), _pin, F(", _channel ="), _channel);
        }

        // Must convert from 16-bit to 8-bit here
        PWMC_SetDutyCycle(PWM_INTERFACE, _channel, _dutycycle >> 8);

        PWM_LOGDEBUG3(F("setupPWM: _channel ="), _channel, F(", _dutycycle ="), _dutycycle);

        return true;
      }

      //////////////////////////////////////////////////

      else if (_pinAttr & PIN_ATTR_TIMER)
      {
        // Use timer clock VARIANT_MCK/2 = 42MHz
        uint32_t TC = VARIANT_MCK / 2 / frequency;

        _dutycycle = dutycycle;

        // Convert to 8-bit
        uint32_t newDC = ((uint32_t) dutycycle >> 8) * TC / MAX_8BIT;
        dutycycle = newDC;

        PWM_LOGDEBUG7(F("setupPWM Timer: _pin ="), _pin, F(", _pinAttr ="), _pinAttr, F(", frequency ="), frequency,
                      F(", dutycycle ="), dutycycle);

        _channel = g_APinDescription[_pin].ulTCChannel;

        if (!_TCChanEnabled[channelToId[_channel]])
        {
          pmc_enable_periph_clk(TC_INTERFACE_ID + channelToId[_channel]);

          TC_Configure(channelToTC[_channel], channelToChNo[_channel],
                       TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_EEVT_XC0 |
                       TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_CLEAR | TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_CLEAR);

          TC_SetRC(channelToTC[_channel], channelToChNo[_channel], TC);

          PWM_LOGDEBUG1(F("setupPWM new Timer, frequency ="), frequency);
        }
        else if (_frequency != frequency)
        {
          TC_SetRC(channelToTC[_channel], channelToChNo[_channel], TC);
          TC_Start(channelToTC[_channel], channelToChNo[_channel]);

          PWM_LOGDEBUG3(F("setupPWM: change frequency to"), frequency, F(" from"), _frequency);
        }

        _frequency = frequency;

        if (dutycycle == 0)
        {
          if (channelToAB[_channel])
          {
            channelToTC[_channel]->TC_CHANNEL[channelToChNo[_channel]].TC_CMR =
              (channelToTC[_channel]->TC_CHANNEL[channelToChNo[_channel]].TC_CMR & 0xFFF0FFFF)
              | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_CLEAR;
          }
          else
          {
            channelToTC[_channel]->TC_CHANNEL[channelToChNo[_channel]].TC_CMR =
              (channelToTC[_channel]->TC_CHANNEL[channelToChNo[_channel]].TC_CMR & 0xF0FFFFFF)
              | TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_CLEAR;
          }
        }
        else
        {
          if (channelToAB[_channel])
          {
            TC_SetRA(channelToTC[_channel], channelToChNo[_channel], dutycycle);
            channelToTC[_channel]->TC_CHANNEL[channelToChNo[_channel]].TC_CMR =
              (channelToTC[_channel]->TC_CHANNEL[channelToChNo[_channel]].TC_CMR & 0xFFF0FFFF)
              | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET;
          }
          else
          {
            TC_SetRB(channelToTC[_channel], channelToChNo[_channel], dutycycle);
            channelToTC[_channel]->TC_CHANNEL[channelToChNo[_channel]].TC_CMR =
              (channelToTC[_channel]->TC_CHANNEL[channelToChNo[_channel]].TC_CMR & 0xF0FFFFFF)
              | TC_CMR_BCPB_CLEAR | TC_CMR_BCPC_SET;
          }
        }

        if ((g_pinStatus[_pin] & 0xF) != PIN_STATUS_PWM)
        {
          PIO_Configure(g_APinDescription[_pin].pPort, g_APinDescription[_pin].ulPinType,
                        g_APinDescription[_pin].ulPin, g_APinDescription[_pin].ulPinConfiguration);
          g_pinStatus[_pin] = (g_pinStatus[_pin] & 0xF0) | PIN_STATUS_PWM;
        }

        if (!_TCChanEnabled[channelToId[_channel]])
        {
          TC_Start(channelToTC[_channel], channelToChNo[_channel]);
          _TCChanEnabled[channelToId[_channel]] = 1;
        }

        _PWMEnabled = true;

        return true;
      }

      _PWMEnabled = false;

      return false;
    }

    ///////////////////////////////////////////
    ///////////////////////////////////////////

  public:

    // dutycycle from 0-65535 for 0%-100%
    bool setPWM_Int(const uint8_t& pin, const float& frequency, const uint16_t& dutycycle)
    {
      if (pin != _pin)
        return false;

      PWM_LOGDEBUG3(F("setPWM_Int: dutycycle ="), dutycycle, F(", frequency ="), frequency);

      return setupPWM(pin, (uint32_t) frequency, dutycycle);
    }

    ///////////////////////////////////////////

    bool setPWM()
    {
      return setPWM_Int(_pin, _frequency, _dutycycle);
    }

    ///////////////////////////////////////////

    bool setPWM(const uint8_t& pin, const float& frequency, const float& dutycycle)
    {
      _dutycycle = round(map(dutycycle, 0, 100.0f, 0, MAX_COUNT_16BIT));

      PWM_LOGDEBUG3(F("setPWM: _dutycycle ="), _dutycycle, F(", frequency ="), frequency);

      return setPWM_Int(pin, frequency, _dutycycle);
    }

    ///////////////////////////////////////////

    bool setPWM_Period(const uint8_t& pin, const float& period_us, const float& dutycycle)
    {
      _dutycycle = round(map(dutycycle, 0, 100.0f, 0, MAX_COUNT_16BIT));

      PWM_LOGDEBUG3(F("setPWM_Period: _dutycycle ="), _dutycycle, F(", period_us ="), period_us);

      return setPWM_Int(pin, round(1000000.0f / period_us), _dutycycle);
    }

    ///////////////////////////////////////////

    // Must have same frequency
    bool setPWM_manual(const uint8_t& pin, const uint16_t& DCValue)
    {
      // Not the same pin or overvalue
      if (_pin != pin)
        return false;

      _dutycycle = DCValue;

      return setupPWM(pin, _frequency, DCValue);
    }

    ///////////////////////////////////////////

    inline float getActualDutyCycle()
    {
      return ( ( (float) _dutycycle + 1 ) * 100 / MAX_16BIT );
    }

    ///////////////////////////////////////////

    inline float getActualFreq()
    {
      return _frequency;
    }

    ///////////////////////////////////////////

    inline float getPWMPeriod()
    {
      return (1000000.0f / _frequency);
    }

    ///////////////////////////////////////////

    inline uint32_t get_freq_CPU()
    {
      return F_CPU;
    }

    ///////////////////////////////////////////

    inline uint32_t getPin()
    {
      return _pin;
    }

    ///////////////////////////////////////////

    bool isPWMEnabled()
    {
      return _PWMEnabled;
    }

    ///////////////////////////////////////////////////////////////////

  private:

    uint32_t      _pinAttr;

    float         _frequency;

    // dutycycle from 0-65535 for 0%-100%
    uint16_t       _dutycycle;

    uint32_t      _channel;

    uint32_t      _pin = INVALID_SAMDUE_PIN;

    bool          _PWMEnabled = false;

    ///////////////////////////////////////////
};

///////////////////////////////////////////


#endif    // SAMDUE_PWM_H

