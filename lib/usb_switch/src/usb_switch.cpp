/**
 * @file       usb_switch.cpp
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for USB Switch Module library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "usb_switch.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method Definitions ---------------------------------- */

UsbSwitch::UsbSwitch(uint8_t pin1, uint8_t pin2) : _pin1(pin1), _pin2(pin2)
{
  pinMode(_pin1, OUTPUT);
  pinMode(_pin2, OUTPUT);
  status[0] = false;
  status[1] = false;
  analogWrite(_pin1, 0);
  analogWrite(_pin2, 0);
}

void UsbSwitch::setOutputValue(uint8_t outputNo, int value)
{
  value = constrain(value, 0, 255);

  if (outputNo == 1 || outputNo == 0)
  {
    analogWrite(_pin1, value);
    status[0] = (value > 0);
  }
  if (outputNo == 2 || outputNo == 0)
  {
    analogWrite(_pin2, value);
    status[1] = (value > 0);
  }
}

void UsbSwitch::setOutputValuePercentage(uint8_t outputNo, int percentage)
{
  percentage   = constrain(percentage, 0, 100);
  int pwmValue = map(percentage, 0, 100, 0, 255);
  setOutputValue(outputNo, pwmValue);
}

int UsbSwitch::getOutputValue(uint8_t outputNo)
{
  if (outputNo == 1)
    return analogRead(_pin1);
  else if (outputNo == 2)
    return analogRead(_pin2);

  return 0;
}

int UsbSwitch::getOutputValuePercentage(uint8_t outputNo)
{
  int pwmValue = getOutputValue(outputNo);
  return map(pwmValue, 0, 255, 0, 100);
}

void UsbSwitch::toggleOutput(uint8_t outputNo)
{
  if (outputNo == 1 || outputNo == 0)
  {
    status[0] = !status[0];
    analogWrite(_pin1, status[0] ? 255 : 0);
  }
  if (outputNo == 2 || outputNo == 0)
  {
    status[1] = !status[1];
    analogWrite(_pin2, status[1] ? 255 : 0);
  }
}

bool UsbSwitch::getStatus(uint8_t outputNo)
{
  if (outputNo == 1)
    return status[0];
  else if (outputNo == 2)
    return status[1];

  return false;
}

/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */
