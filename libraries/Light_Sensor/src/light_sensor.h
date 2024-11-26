/**
 * @file       light_sensor.h
 * @version    1.0.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header file for Light Sensor driver
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

/* Includes ----------------------------------------------------------- */
#include "Arduino.h"

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
class LightSensor
{
 public:
  /**
   * @brief  init the Light Sensor with a specified pin
   */
  LightSensor(int pin);

  /**
   * @brief  Read the analog value from the sensor using the arduino function analogRead
   *
   * @param[in]  void
   *
   * @return     int            The analog value read from the sensor.
   *
   */
  int read();

  /**
   * @brief Read the sensor and map its raw analog values to a range (e.g., 0-100).
   *
   * @param[in]   minValue      Minimum value of the range
   * @param[in]   maxValue      Maximum value of the range
   *
   * @return      int           The calibrated sensor value mapped to a range of minValue to maxValue.
   */
  int readAndMap(int minValue, int maxValue);

 private:
  int      _pin;        /** SIG pin of the Light Sensor */
  uint32_t sensorValue; /** Value from the sensor */
};

#endif  // LIGHT_SENSOR_H

/* End of file -------------------------------------------------------- */