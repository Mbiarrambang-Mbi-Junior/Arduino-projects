# SAMDUE_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/SAMDUE_PWM.svg?)](https://www.ardu-badge.com/SAMDUE_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/SAMDUE_PWM.svg)](https://github.com/khoih-prog/SAMDUE_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/SAMDUE_PWM/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/SAMDUE_PWM.svg)](http://github.com/khoih-prog/SAMDUE_PWM/issues)


<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>


---
---

## Table of Contents

* [Why do we need this SAMDUE_PWM library](#why-do-we-need-this-SAMDUE_PWM-library)
  * [Features](#features)
  * [Why using hardware-based PWM is better](#why-using-hardware-based-pwm-is-better)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Usage](#usage)
  * [1. Create PWM Instance with Pin, Frequency, dutycycle](#1-create-pwm-instance-with-pin-frequency-dutycycle)
  * [2. Initialize PWM Instance](#2-Initialize-PWM-Instance)
  * [3. Set or change PWM frequency or dutyCycle](#3-set-or-change-PWM-frequency-or-dutyCycle)
  * [4. Set or change PWM frequency and dutyCycle manually and efficiently in waveform creation](#4-Set-or-change-PWM-frequency-and-dutyCycle-manually-and-efficiently-in-waveform-creation)
* [Examples](#examples)
  * [ 1. PWM_Basic](examples/PWM_Basic)
  * [ 2. PWM_DynamicDutyCycle](examples/PWM_DynamicDutyCycle) 
  * [ 3. PWM_DynamicDutyCycle_Int](examples/PWM_DynamicDutyCycle_Int)
  * [ 4. PWM_DynamicFreq](examples/PWM_DynamicFreq)
  * [ 5. PWM_Multi](examples/PWM_Multi)
  * [ 6. PWM_MultiChannel](examples/PWM_MultiChannel)
  * [ 7. PWM_Waveform](examples/PWM_Waveform)
  * [ 8. PWM_StepperControl](examples/PWM_StepperControl) **New**
* [Example PWM_Multi](#example-PWM_Multi)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. PWM_DynamicDutyCycle using PWM on SAM_DUE](#1-PWM_DynamicDutyCycle-using-PWM-on-SAM_DUE)
  * [2. PWM_Multi using Timer on SAM_DUE](#2-PWM_Multi-using-Timer-on-SAM_DUE)
  * [3. PWM_DynamicFreq using Timer on SAM_DUE](#3-PWM_DynamicFreq-using-Timer-on-SAM_DUE)
  * [4. PWM_Waveform using PWM on SAM_DUE](#4-PWM_Waveform-using-PWM-on-SAM_DUE)
  * [5. PWM_Waveform using Timer on SAM_DUE](#5-PWM_Waveform-using-Timer-on-SAM_DUE)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---


### Why do we need this [SAMDUE_PWM library](https://github.com/khoih-prog/SAMDUE_PWM)

### Features

This hardware-based PWM library enables you to use Hardware-PWM on **SAM_DUE boards** to create and output PWM. These purely hardware-based PWM channels can generate very high PWM frequencies, depending on CPU clock and acceptable accuracy. The maximum resolution is only **8-bit** resolution.

This library is based on `analogWrite()` function in [`wiring_analog.c`](https://github.com/arduino/ArduinoCore-sam/blob/master/cores/arduino/wiring_analog.c) of Arduino SAM_DUE core, to enable dynamic frequencies and dutycycles.

This library is using the **same or similar functions** as other FastPWM libraries, as follows, to enable you to **port your PWM code easily between platforms**

 1. [**RP2040_PWM**](https://github.com/khoih-prog/RP2040_PWM)
 2. [**AVR_PWM**](https://github.com/khoih-prog/AVR_PWM)
 3. [**megaAVR_PWM**](https://github.com/khoih-prog/megaAVR_PWM)
 4. [**ESP32_FastPWM**](https://github.com/khoih-prog/ESP32_FastPWM)
 5. [**SAMD_PWM**](https://github.com/khoih-prog/SAMD_PWM)
 6. [**SAMDUE_PWM**](https://github.com/khoih-prog/SAMDUE_PWM)
 7. [**nRF52_PWM**](https://github.com/khoih-prog/nRF52_PWM)
 8. [**Teensy_PWM**](https://github.com/khoih-prog/Teensy_PWM)
 9. [**ATtiny_PWM**](https://github.com/khoih-prog/ATtiny_PWM)
10. [**Dx_PWM**](https://github.com/khoih-prog/Dx_PWM)
11. [**Portenta_H7_PWM**](https://github.com/khoih-prog/Portenta_H7_PWM)
12. [**MBED_RP2040_PWM**](https://github.com/khoih-prog/MBED_RP2040_PWM)
13. [**nRF52_MBED_PWM**](https://github.com/khoih-prog/nRF52_MBED_PWM)
14. [**STM32_PWM**](https://github.com/khoih-prog/STM32_PWM)


---

The most important feature is they're purely hardware-based PWM channels. Therefore, their operations are **not blocked by bad-behaving software functions / tasks**.

This important feature is absolutely necessary for mission-critical tasks. These hardware PWM-channels, still work even if other software functions are blocking. Moreover, they are much more precise (certainly depending on clock frequency accuracy) than other software timers using `millis()` or `micros()`. That's necessary if you need to control external systems (Servo, etc.) requiring better accuracy.

New efficient `setPWM_manual()` function enables waveform creation using PWM.

The [**PWM_Multi**](examples/PWM_Multi) example will demonstrate the usage of multichannel PWM using multiple Hardware-PWM blocks (Timer & Channel). The 4 independent Hardware-PWM channels are used **to control 4 different PWM outputs**, with totally independent frequencies and dutycycles on `SAM_DUE`.

Being hardware-based PWM, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet or Blynk services.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

---

### Why using hardware-based PWM is better

Imagine you have a system with a **mission-critical** function, controlling a robot or doing something much more important. You normally use a software timer to poll, or even place the function in `loop()`. But what if another function is **blocking** the `loop()` or `setup()`.

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use `hardware-based PWM`.

These hardware-based PWM channels still work even if other software functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software-based PWMs, using `millis()` or `micros()`.

Functions using normal software-based PWMs, relying on `loop()` and calling `millis()`, won't work if the `loop()` or `setup()` is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

---

### Currently supported Boards

1. **SAM_DUE** boards, using [`Arduino SAM core`](https://github.com/arduino/ArduinoCore-sam)


---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Arduino SAM core v1.6.12+`](https://github.com/arduino/ArduinoCore-sam)

  
   
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**SAMDUE_PWM**](https://github.com/khoih-prog/SAMDUE_PWM), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/SAMDUE_PWM.svg?)](https://www.ardu-badge.com/SAMDUE_PWM) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**SAMDUE_PWM**](https://github.com/khoih-prog/SAMDUE_PWM) page.
2. Download the latest release `SAMDUE_PWM-main.zip`.
3. Extract the zip file to `SAMDUE_PWM-main` directory 
4. Copy whole `SAMDUE_PWM-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**SAMDUE_PWM** library](https://registry.platformio.org/libraries/khoih-prog/SAMDUE_PWM) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/SAMDUE_PWM/installation). Search for **SAMDUE_PWM** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---


## Usage

Before using any PWM `Timer` and `channel`, you have to make sure the `Timer` and `channel` has not been used by any other purpose.

```cpp
// SAM_DUE:
// PWM pins: 6, 7, 8, 9
// Timer pins: 2-5, 10-13. 
// pin2:   TC0_CHA0, pin3:  TC2_CHA7, pin4:  TC2_CHB6, pin5:  TC2_CHA6
// pin 10: TC2_CHB7, pin11: TC2_CHA8, pin12: TC2_CHB8, pin13: TC0_CHB0
```

#### 1. Create PWM Instance with Pin, Frequency, dutycycle

```cpp
SAMDUE_PWM* PWM_Instance;

PWM_Instance = new SAMDUE_PWM(pinToUse, frequency, dutyCycle, channel, PWM_resolution);
```

#### 2. Initialize PWM Instance

```cpp
if (PWM_Instance)
{
  PWM_Instance->setPWM();
}
```

#### 3. Set or change PWM frequency or dutyCycle

To use `float new_dutyCycle`

```cpp
PWM_Instance->setPWM(PWM_Pins, new_frequency, new_dutyCycle);
```

such as 

```cpp
dutyCycle = 10.0f;
  
Serial.print(F("Change PWM DutyCycle to ")); Serial.println(dutyCycle);
PWM_Instance->setPWM(pinToUse, frequency, dutyCycle);
```

---

To use `uint32_t new_dutyCycle` = `(real_dutyCycle * 65536) / 100`


```cpp
PWM_Instance->setPWM_Int(PWM_Pins, new_frequency, new_dutyCycle);
```

such as for `real_dutyCycle = 50%`

```cpp
// 50% dutyCycle = (real_dutyCycle * 65536) / 100
dutyCycle = 32768;

Serial.print(F("Change PWM DutyCycle to (%) "));
Serial.println((float) dutyCycle * 100 / 65536);
PWM_Instance->setPWM_Int(pinToUse, frequency, dutyCycle);
```

for `real_dutyCycle = 50%`

```cpp
// 20% dutyCycle = (real_dutyCycle * 65536) / 100
dutyCycle = 13107;

Serial.print(F("Change PWM DutyCycle to (%) "));
Serial.println((float) dutyCycle * 100 / 65536);
PWM_Instance->setPWM_Int(pinToUse, frequency, dutyCycle);
```

#### 4. Set or change PWM frequency and dutyCycle manually and efficiently in waveform creation

Function prototype

```cpp
bool setPWM_manual(const uint8_t& pin, const uint16_t& DCValue);
```

Need to call only once for each pin


```cpp
PWM_Instance->setPWM(PWM_Pins, frequency, dutyCycle);
```

after that, if just changing `dutyCycle` / `level`, use 

```cpp
PWM_Instance->setPWM_manual(PWM_Pins, new_level);
```

---
---

### Examples: 

 1. [PWM_Basic](examples/PWM_Basic)
 2. [PWM_DynamicDutyCycle](examples/PWM_DynamicDutyCycle)
 3. [PWM_DynamicDutyCycle_Int](examples/PWM_DynamicDutyCycle_Int)
 4. [PWM_DynamicFreq](examples/PWM_DynamicFreq)
 5. [PWM_Multi](examples/PWM_Multi)
 6. [PWM_MultiChannel](examples/PWM_MultiChannel)
 7. [PWM_Waveform](examples/PWM_Waveform)
 8. [PWM_StepperControl](examples/PWM_StepperControl) **New**

 
---
---

### Example [PWM_Multi](examples/PWM_Multi)

https://github.com/khoih-prog/SAMDUE_PWM/blob/a5ac09f989e8377b3ba14c68015edd18510f439b/examples/PWM_Multi/PWM_Multi.ino#L11-L117


---
---

### Debug Terminal Output Samples

### 1. PWM_DynamicDutyCycle using PWM on SAM_DUE

The following is the sample terminal output when running example [PWM_DynamicDutyCycle](examples/PWM_DynamicDutyCycle) on **SAM_DUE**, to demonstrate the ability to provide high PWM frequencies and ability to change DutyCycle `on-the-fly` using **PWM**


```cpp
Starting PWM_DynamicDutyCycle using PWM on SAM_DUE
SAMDUE_PWM v1.0.1
[PWM] setupPWM: _pin = 6 , _pinAttr = 12 , frequency = 5000 , _dutycycle = 0
[PWM] setupPWM: Not PWMEnabled, PWMC_ConfigureClocks, frequency = 5000
[PWM] setupPWM: new _pin = 6 , _channel = 7
[PWM] setupPWM: _channel = 7 , _dutycycle = 0
=====================================================================================
Change PWM DutyCycle to 90.00
[PWM] setPWM: _dutycycle = 58982 , frequency = 5000.00
[PWM] setPWM_Int: dutycycle = 58982 , frequency = 5000.00
[PWM] setupPWM: _pin = 6 , _pinAttr = 12 , frequency = 5000 , _dutycycle = 58982
[PWM] setupPWM: _channel = 7 , _dutycycle = 58982
=====================================================================================
Actual data: pin = 6, PWM DC = 90.00, PWMPeriod = 200.00, PWM Freq (Hz) = 5000.0000
=====================================================================================
Change PWM DutyCycle to 20.00
[PWM] setPWM: _dutycycle = 13107 , frequency = 5000.00
[PWM] setPWM_Int: dutycycle = 13107 , frequency = 5000.00
[PWM] setupPWM: _pin = 6 , _pinAttr = 12 , frequency = 5000 , _dutycycle = 13107
[PWM] setupPWM: _channel = 7 , _dutycycle = 13107
=====================================================================================
Actual data: pin = 6, PWM DC = 20.00, PWMPeriod = 200.00, PWM Freq (Hz) = 5000.0000
=====================================================================================
Change PWM DutyCycle to 90.00
[PWM] setPWM: _dutycycle = 58982 , frequency = 5000.00
[PWM] setPWM_Int: dutycycle = 58982 , frequency = 5000.00
[PWM] setupPWM: _pin = 6 , _pinAttr = 12 , frequency = 5000 , _dutycycle = 58982
[PWM] setupPWM: _channel = 7 , _dutycycle = 58982
=====================================================================================
Actual data: pin = 6, PWM DC = 90.00, PWMPeriod = 200.00, PWM Freq (Hz) = 5000.0000
=====================================================================================
Change PWM DutyCycle to 20.00
[PWM] setPWM: _dutycycle = 13107 , frequency = 5000.00
[PWM] setPWM_Int: dutycycle = 13107 , frequency = 5000.00
[PWM] setupPWM: _pin = 6 , _pinAttr = 12 , frequency = 5000 , _dutycycle = 13107
[PWM] setupPWM: _channel = 7 , _dutycycle = 13107
=====================================================================================
Actual data: pin = 6, PWM DC = 20.00, PWMPeriod = 200.00, PWM Freq (Hz) = 5000.0000
=====================================================================================
```

---

### 2. PWM_Multi using Timer on SAM_DUE

The following is the sample terminal output when running example [**PWM_Multi**](examples/PWM_Multi) on **SAM_DUE**, to demonstrate the ability to provide high PWM frequencies on multiple `PWM-capable` pins using **Timer** for PWM

```cpp
Starting PWM_Multi using Timer on SAM_DUE
SAMDUE_PWM v1.0.1
[PWM] setupPWM Timer: _pin = 2 , _pinAttr = 20 , frequency = 2000 , dutycycle = 2058
[PWM] setupPWM new Timer, frequency = 2000
[PWM] setupPWM Timer: _pin = 2 , _pinAttr = 20 , frequency = 2000 , dutycycle = 2058
[PWM] setupPWM Timer: _pin = 3 , _pinAttr = 20 , frequency = 3000 , dutycycle = 4172
[PWM] setupPWM new Timer, frequency = 3000
[PWM] setupPWM Timer: _pin = 3 , _pinAttr = 20 , frequency = 3000 , dutycycle = 4172
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 4000 , dutycycle = 5270
[PWM] setupPWM new Timer, frequency = 4000
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 4000 , dutycycle = 5270
[PWM] setupPWM Timer: _pin = 11 , _pinAttr = 20 , frequency = 8000 , dutycycle = 4735
[PWM] setupPWM new Timer, frequency = 8000
[PWM] setupPWM Timer: _pin = 11 , _pinAttr = 20 , frequency = 8000 , dutycycle = 4735
=====================================================================================
Index	Pin	PWM_freq	DutyCycle	Actual Freq
=====================================================================================
0	2	2000.00		10.00		2000.0000
1	3	3000.00		30.00		3000.0000
2	5	4000.00		50.00		4000.0000
3	11	8000.00		90.00		8000.0000
=====================================================================================
Actual data: pin = 2, PWM DC = 10.00, PWMPeriod = 500.00, PWM Freq (Hz) = 2000.0000
=====================================================================================
=====================================================================================
Actual data: pin = 3, PWM DC = 30.00, PWMPeriod = 333.33, PWM Freq (Hz) = 3000.0000
=====================================================================================
=====================================================================================
Actual data: pin = 5, PWM DC = 50.00, PWMPeriod = 250.00, PWM Freq (Hz) = 4000.0000
=====================================================================================
=====================================================================================
Actual data: pin = 11, PWM DC = 90.00, PWMPeriod = 125.00, PWM Freq (Hz) = 8000.0000
=====================================================================================
```

---

### 3. PWM_DynamicFreq using Timer on SAM_DUE

The following is the sample terminal output when running example [**PWM_DynamicFreq**](examples/PWM_DynamicFreq) on **SAM_DUE**, to demonstrate the ability to change dynamically PWM frequencies using **Timer** for PWM

```cpp
Starting PWM_DynamicFreq using Timer on SAM_DUE
SAMDUE_PWM v1.0.1
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 10000 , dutycycle = 2108
[PWM] setupPWM new Timer, frequency = 10000
=====================================================================================
Change PWM Freq to 20000.00
[PWM] setPWM: _dutycycle = 32768 , frequency = 20000.00
[PWM] setPWM_Int: dutycycle = 32768 , frequency = 20000.00
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 20000 , dutycycle = 1054
[PWM] setupPWM: change frequency to 20000  from 10000.00
=====================================================================================
Actual data: pin = 5, PWM DC = 50.00, PWMPeriod = 50.00, PWM Freq (Hz) = 20000.0000
=====================================================================================
Change PWM Freq to 10000.00
[PWM] setPWM: _dutycycle = 32768 , frequency = 10000.00
[PWM] setPWM_Int: dutycycle = 32768 , frequency = 10000.00
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 10000 , dutycycle = 2108
[PWM] setupPWM: change frequency to 10000  from 20000.00
=====================================================================================
Actual data: pin = 5, PWM DC = 50.00, PWMPeriod = 100.00, PWM Freq (Hz) = 10000.0000
=====================================================================================
Change PWM Freq to 20000.00
[PWM] setPWM: _dutycycle = 32768 , frequency = 20000.00
[PWM] setPWM_Int: dutycycle = 32768 , frequency = 20000.00
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 20000 , dutycycle = 1054
[PWM] setupPWM: change frequency to 20000  from 10000.00
=====================================================================================
Actual data: pin = 5, PWM DC = 50.00, PWMPeriod = 50.00, PWM Freq (Hz) = 20000.0000
=====================================================================================
```

---


### 4. PWM_Waveform using PWM on SAM_DUE

The following is the sample terminal output when running example [**PWM_Waveform**](examples/PWM_Waveform) on **SAM_DUE**, to demonstrate how to use the `setPWM_manual()` function in wafeform creation using **PWM**  for PWM


```cpp
Starting PWM_Waveform using PWM on SAM_DUE
SAMDUE_PWM v1.0.1
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 0
[PWM] setupPWM: Not PWMEnabled, PWMC_ConfigureClocks, frequency = 2000
[PWM] setupPWM: new _pin = 6 , _channel = 7
[PWM] setupPWM: _channel = 7 , _dutycycle = 0
[PWM] setPWM: _dutycycle = 0 , frequency = 2000.00
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 0
[PWM] setupPWM: _channel = 7 , _dutycycle = 0
============================================================================================
Actual data: pin = 6, PWM DutyCycle = 0.00, PWMPeriod = 500.00, PWM Freq (Hz) = 2000.0000
============================================================================================
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 0
[PWM] setupPWM: _channel = 7 , _dutycycle = 0
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 3276
[PWM] setupPWM: _channel = 7 , _dutycycle = 3276
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 6553
[PWM] setupPWM: _channel = 7 , _dutycycle = 6553
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 9830
[PWM] setupPWM: _channel = 7 , _dutycycle = 9830
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 13107
[PWM] setupPWM: _channel = 7 , _dutycycle = 13107
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 16383
[PWM] setupPWM: _channel = 7 , _dutycycle = 16383
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 19660
[PWM] setupPWM: _channel = 7 , _dutycycle = 19660
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 22937
[PWM] setupPWM: _channel = 7 , _dutycycle = 22937
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 26214
[PWM] setupPWM: _channel = 7 , _dutycycle = 26214
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 29490
[PWM] setupPWM: _channel = 7 , _dutycycle = 29490
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 32767
[PWM] setupPWM: _channel = 7 , _dutycycle = 32767
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 36044
[PWM] setupPWM: _channel = 7 , _dutycycle = 36044
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 39321
[PWM] setupPWM: _channel = 7 , _dutycycle = 39321
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 42597
[PWM] setupPWM: _channel = 7 , _dutycycle = 42597
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 45874
[PWM] setupPWM: _channel = 7 , _dutycycle = 45874
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 49151
[PWM] setupPWM: _channel = 7 , _dutycycle = 49151
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 52428
[PWM] setupPWM: _channel = 7 , _dutycycle = 52428
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 55704
[PWM] setupPWM: _channel = 7 , _dutycycle = 55704
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 58981
[PWM] setupPWM: _channel = 7 , _dutycycle = 58981
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 62258
[PWM] setupPWM: _channel = 7 , _dutycycle = 62258
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 65535
[PWM] setupPWM: _channel = 7 , _dutycycle = 65535
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 62258
[PWM] setupPWM: _channel = 7 , _dutycycle = 62258
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 58981
[PWM] setupPWM: _channel = 7 , _dutycycle = 58981
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 55704
[PWM] setupPWM: _channel = 7 , _dutycycle = 55704
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 52428
[PWM] setupPWM: _channel = 7 , _dutycycle = 52428
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 49151
[PWM] setupPWM: _channel = 7 , _dutycycle = 49151
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 45874
[PWM] setupPWM: _channel = 7 , _dutycycle = 45874
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 42597
[PWM] setupPWM: _channel = 7 , _dutycycle = 42597
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 39321
[PWM] setupPWM: _channel = 7 , _dutycycle = 39321
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 36044
[PWM] setupPWM: _channel = 7 , _dutycycle = 36044
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 32767
[PWM] setupPWM: _channel = 7 , _dutycycle = 32767
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 29490
[PWM] setupPWM: _channel = 7 , _dutycycle = 29490
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 26214
[PWM] setupPWM: _channel = 7 , _dutycycle = 26214
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 22937
[PWM] setupPWM: _channel = 7 , _dutycycle = 22937
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 19660
[PWM] setupPWM: _channel = 7 , _dutycycle = 19660
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 16383
[PWM] setupPWM: _channel = 7 , _dutycycle = 16383
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 13107
[PWM] setupPWM: _channel = 7 , _dutycycle = 13107
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 9830
[PWM] setupPWM: _channel = 7 , _dutycycle = 9830
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 6553
[PWM] setupPWM: _channel = 7 , _dutycycle = 6553
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 3276
[PWM] setupPWM: _channel = 7 , _dutycycle = 3276
[PWM] setupPWM: _pinAttr = 12 , frequency = 2000 , _dutycycle = 0
[PWM] setupPWM: _channel = 7 , _dutycycle = 0
```

---

### 5. PWM_Waveform using Timer on SAM_DUE

The following is the sample terminal output when running example [**PWM_Waveform**](examples/PWM_Waveform) on **SAM_DUE**, to demonstrate how to use the `setPWM_manual()` function in wafeform creation using **Timer**  for PWM


```cpp
Starting PWM_Waveform using Timer on SAM_DUE
SAMDUE_PWM v1.0.1
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 0
[PWM] setupPWM new Timer, frequency = 2000
[PWM] setPWM: _dutycycle = 0 , frequency = 2000.00
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 0
============================================================================================
Actual data: pin = 5, PWM DutyCycle = 0.00, PWMPeriod = 500.00, PWM Freq (Hz) = 2000.0000
============================================================================================
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 0
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 988
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 2058
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 3129
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 4200
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 5188
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 6258
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 7329
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 8400
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 9470
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 10458
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 11529
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 12600
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 13670
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 14741
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 15729
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 16800
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 17870
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 18941
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 20011
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 21000
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 20011
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 18941
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 17870
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 16800
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 15729
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 14741
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 13670
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 12600
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 11529
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 10458
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 9470
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 8400
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 7329
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 6258
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 5188
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 4200
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 3129
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 2058
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 988
[PWM] setupPWM Timer: _pin = 5 , _pinAttr = 20 , frequency = 2000 , dutycycle = 0
```

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level `_PWM_LOGLEVEL_` from 0 to 4

```cpp
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [SAMDUE_PWM issues](https://github.com/khoih-prog/SAMDUE_PWM/issues)

---
---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards

---

## DONE

 1. Basic hardware PWM-channels for **SAM_DUE boards** using [`Arduino SAM core`](https://github.com/arduino/ArduinoCore-sam)
 2. Add example [PWM_StepperControl](https://github.com/khoih-prog/SAMDUE_PWM/tree/main/examples/PWM_StepperControl) to demo how to control Stepper Motor using PWM
 
 
---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

1. Thanks to [Paul van Dinther](https://github.com/dinther) for proposing new way to use PWM to drive Stepper-Motor in [Using PWM to step a stepper driver #16](https://github.com/khoih-prog/RP2040_PWM/issues/16), leading to v2.0.3


<table>
  <tr>
    <td align="center"><a href="https://github.com/dinther"><img src="https://github.com/dinther.png" width="100px;" alt="dinther"/><br /><sub><b>Paul van Dinther</b></sub></a><br /></td>
  </tr>
</table>


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/SAMDUE_PWM/blob/main/LICENSE)

---

## Copyright

Copyright (c) 2022- Khoi Hoang


