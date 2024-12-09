/**
 * @file       soil_moisture.cpp
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for soil moisture sensor library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "soil_moisture.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method Definitions ---------------------------------- */

// Constructor
SoilMoisture::SoilMoisture(int pin) : _pin(pin), sensorValue(0) { pinMode(_pin, INPUT); }

uint32_t SoilMoisture::read()
{
  sensorValue = analogRead(_pin);
  return sensorValue;
}

uint32_t SoilMoisture::readAndMap(int minValue, int maxValue)
{
  // Read the raw sensor value
  sensorValue = read();

  // Map the sensor value from 0-1023 to a range of minValue - maxValue
  uint32_t mappedValue = map(sensorValue, 0, 1023, minValue, maxValue);
  mappedValue          = constrain(mappedValue, minValue, maxValue);

  return mappedValue;
}

uint8_t SoilMoisture::convertToPercentage(int value)
{
  value = map(value, 0, 1023, 0, 100);
  return constrain(value, 0, 100);
}

bool SoilMoisture::isAboveThreshold(int threshold) { return read() > threshold; }

int SoilMoisture::getAverageReading(int samples)
{
  long total = 0;
  for (int i = 0; i < samples; i++)
  {
    total += read();
    delay(10); // Small delay between readings
  }
  return total / samples;
}

void SoilMoisture::onThresholdCross(int threshold, void (*callback)())
{
  _callback = callback;
  if (isAboveThreshold(threshold) && _callback != nullptr)
  {
    _callback();
  }
}

/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */
