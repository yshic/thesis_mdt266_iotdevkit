/**
 * @file       pir_sensor.cpp
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for PIR Sensor Library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "pir_sensor.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method Definitions ---------------------------------- */

// Constructor
PIRSensor::PIRSensor(uint8_t pin)
    : _pin(pin), status(0), _lastMotionTime(0), _debounceTime(0), _motionCallback(nullptr)
{
  pinMode(_pin, INPUT);
}

void PIRSensor::read()
{
  unsigned long currentTime = millis();
  if ((currentTime - _lastMotionTime) < _debounceTime)
  {
    status = 0;
    return; // Debounce in effect, ignore motion
  }

  int motionDetected = digitalRead(_pin);
  if (motionDetected == HIGH)
  {
    status          = 1;
    _lastMotionTime = currentTime;
    if (_motionCallback)
    {
      _motionCallback();
    }
    return;
  }
  status = 0;
}

uint8_t PIRSensor::getStatus() { return status; }

int PIRSensor::setDebounceTime(unsigned long debounceTime)
{
  if (debounceTime < 0)
  {
    return 1; // Error: Invalid debounce time
  }
  _debounceTime = debounceTime;
  return 0;
}

int PIRSensor::isMotionContinuous(unsigned long duration)
{
  unsigned long startTime = millis();
  while (millis() - startTime < duration)
  {
    if (digitalRead(_pin) == LOW)
    {
      return 0; // No continuous motion detected
    }
  }
  return 1; // Motion detected continuously for the duration
}

unsigned long PIRSensor::getLastMotionTime() { return _lastMotionTime; }

int PIRSensor::setMotionCallback(void (*callback)())
{
  if (!callback)
  {
    return 1; // Error: Invalid callback function
  }
  _motionCallback = callback;
  return 0;
}

int PIRSensor::disableMotionCallback()
{
  _motionCallback = nullptr;
  return 0;
}

int PIRSensor::setSensitivity(int level)
{
  // Assuming sensitivity can be mapped to debounce time for simplicity
  switch (level)
  {
    case 1:
      _debounceTime = 1000; // Low sensitivity
      break;
    case 2:
      _debounceTime = 500; // Medium sensitivity
      break;
    case 3:
      _debounceTime = 200; // High sensitivity
      break;
    default:
      return 1; // Error: Invalid sensitivity level
  }
  return 0;
}

int PIRSensor::reset()
{
  status          = 0;
  _lastMotionTime = 0;
  _debounceTime   = 0;
  _motionCallback = nullptr;
  return 0;
}

/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */
