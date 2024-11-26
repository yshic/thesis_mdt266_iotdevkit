/**
 * @file       light_sensor.cpp
 * @version    1.0.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for light sensor
 *
 */

/* Includes ----------------------------------------------------------- */
#include "light_sensor.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class Member Function Definitions ---------------------------------- */
LightSensor::LightSensor(int pin) : _pin(pin), sensorValue(0) {}

int LightSensor::read()
{
  // Implementation of public function
  sensorValue = analogRead(_pin);
  return sensorValue;
}

int LightSensor::readAndMap(int minValue, int maxValue)
{
  // Read the raw sensor value
  sensorValue = read();
  // Map the sensor value from 0-1023 to a range of minValue - maxValue
  int mappedValue = map(sensorValue, 0, 1023, minValue, maxValue);
  mappedValue     = constrain(mappedValue, 0, 100);
  return mappedValue;
}

/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */
