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
#include "bsp_gpio.h"

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
  bspGpioPinMode(_pin, INPUT);
}

void PIRSensor::read()
{
  unsigned long currentTime = millis();
  if ((currentTime - _lastMotionTime) < _debounceTime)
  {
    status = 0;
    return; // Debounce in effect, ignore motion
  }

  int motionDetected = bspGpioDigitalRead(_pin);
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

pir_sensor_error_t PIRSensor::setDebounceTime(unsigned long debounceTime)
{
  if (debounceTime < 0)
  {
    return PIR_SENSOR_ERR; // Error: Invalid debounce time
  }
  _debounceTime = debounceTime;
  return PIR_SENSOR_OK;
}

int PIRSensor::isMotionContinuous(unsigned long duration)
{
  unsigned long startTime = millis();
  while (millis() - startTime < duration)
  {
    if (bspGpioDigitalRead(_pin) == LOW)
    {
      return 0; // No continuous motion detected
    }
  }
  return 1; // Motion detected continuously for the duration
}

unsigned long PIRSensor::getLastMotionTime() { return _lastMotionTime; }

pir_sensor_error_t PIRSensor::setMotionCallback(void (*callback)())
{
  if (!callback)
  {
    return PIR_SENSOR_ERR; // Error: Invalid callback function
  }
  _motionCallback = callback;
  return PIR_SENSOR_OK;
}

pir_sensor_error_t PIRSensor::disableMotionCallback()
{
  _motionCallback = nullptr;
  return PIR_SENSOR_OK;
}

pir_sensor_error_t PIRSensor::setSensitivity(int level)
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
      return PIR_SENSOR_ERR; // Error: Invalid sensitivity level
  }
  return PIR_SENSOR_OK;
}

int PIRSensor::reset()
{
  status          = 0;
  _lastMotionTime = 0;
  _debounceTime   = 0;
  _motionCallback = nullptr;
  return PIR_SENSOR_OK;
}

/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */
