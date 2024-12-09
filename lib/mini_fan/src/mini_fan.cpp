/**
 * @file       mini_fan.cpp
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for Mini Fan Module Library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "mini_fan.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method Definitions ---------------------------------- */

// Constructor
MiniFan::MiniFan(int pin) : _pin(pin)
{
  pinMode(_pin, OUTPUT);
  _speed = 0;
  status = false;
}

void MiniFan::setFanSpeed(int speed)
{
  _speed = speed;

  // Change status of the fan
  if (_speed > 0)
  {
    status = true;
  }
  else
  {
    status = false;
  }

  // Spin the fan
  analogWrite(_pin, _speed);
}

void MiniFan::setFanSpeedPercentage(int percentage)
{
  // Ensure the input percentage is within 0-100
  percentage = constrain(percentage, 0, 100);

  // Map the percentage to a range of 0-255
  int pwmValue = map(percentage, 0, 100, 0, 255);

  // Set the fan speed using the mapped PWM value
  analogWrite(_pin, pwmValue);

  // Update the fan state
  status = (pwmValue > 0);
}

int MiniFan::getFanSpeed() { return _speed; }

int MiniFan::getFanSpeedPercentage()
{
  int percentage = map(_speed, 0, 255, 0, 100);
  return constrain(percentage, 0, 100);
}

void MiniFan::toggleFan()
{
  if (isFanRunning())
  {
    digitalWrite(_pin, LOW);
  }
  else
  {
    digitalWrite(_pin, HIGH);
  }
  status = !status;
}

bool MiniFan::isFanRunning() { return status; }

/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */
