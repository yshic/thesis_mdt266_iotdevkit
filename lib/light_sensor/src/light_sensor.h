/**
 * @file       light_sensor.h
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header for Light Sensor Library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

/* Includes ----------------------------------------------------------- */
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

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

  /**
   * @brief Check if the light intensity is above or below a threshold.
   *
   * @param[in] threshold   Threshold value for light intensity.
   *
   * @return bool           True if light intensity is above the threshold, false otherwise.
   */
  bool isAboveThreshold(int threshold);

  /**
   * @brief Get the average sensor reading over a specified number of samples.
   *
   * @param[in] samples   Number of samples to average.
   *
   * @return int          The averaged sensor value.
   */
  int getAverageReading(int samples = 10);

  /**
   * @brief Set a callback function to trigger when light intensity crosses a threshold.
   *
   * @param[in] threshold Threshold for triggering the callback.
   * @param[in] callback  Function to call when threshold is crossed.
   */
  void onThresholdCross(int threshold, void (*callback)());

private:
  int      _pin;                 /** SIG pin of the Light Sensor */
  uint32_t sensorValue;          /** Value from the sensor */
  void (*_callback)() = nullptr; /** Callback function */
};

#endif // LIGHT_SENSOR_H

/* End of file -------------------------------------------------------- */