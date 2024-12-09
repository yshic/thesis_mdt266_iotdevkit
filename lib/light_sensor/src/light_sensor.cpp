/**
 * @file       light_sensor.cpp
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for Light Sensor Library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "light_sensor.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method Definitions ---------------------------------- */

// Constructor
LightSensor::LightSensor(int pin) : _pin(pin), sensorValue(0)
{
  pinMode(_pin, INPUT);
  // Change the ADC resolution to 12 bits
  analogReadResolution(12);
}

int LightSensor::read()
{
  sensorValue = analogRead(_pin);
  return sensorValue;
}

int LightSensor::readAndMap(int minValue, int maxValue)
{
  // Read the raw sensor value
  sensorValue = read();
  // Map the sensor value from 0-4095 to a range of minValue - maxValue
  int mappedValue = map(sensorValue, 0, 4095, minValue, maxValue);
  mappedValue     = constrain(mappedValue, 0, 100);
  return mappedValue;
}

bool LightSensor::isAboveThreshold(int threshold) { return read() > threshold; }

int LightSensor::getAverageReading(int samples)
{
  long total = 0;
  for (int i = 0; i < samples; i++)
  {
    total += read();
    delay(10); // Small delay between readings
  }
  return total / samples;
}

void LightSensor::onThresholdCross(int threshold, void (*callback)())
{
  _callback = callback;
  if (isAboveThreshold(threshold) && _callback != nullptr)
  {
    _callback();
  }
}

/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */
